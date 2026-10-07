#include "main.h"

//-----Macro-----//
//-----Torque_Nm-----
#define ADC_SAMPLE_TIME	100
#define Torque_Filter		30
#define Torque_DEADBAND	0.2f

//-----Hub Motor-----
#define H_Poles_Pair		15
#define R_Tire					233.4f	//mm

//-----Load Motor-----
#define L_Poles_Pair		4

//-----Load level-----
#define No_Load					0.0f
#define Low_Load 				12.5f
#define Medium_Load			20.0f
#define High_Load				30.0f
volatile uint8_t Num_Load_Level;

//-----Telemetry------
#define Header_Frame_1	0x67
#define Header_Frame_2	0x89
#define End_Frame				0xFF

//-----ADC Sample Time-----
static volatile uint16_t adc_value[ADC_SAMPLE_TIME]; //Read Analog from PA1

	bool DMA_ADC1_Process(void){
		if((DMA_ISR & (0x01 << 1)) == 0){
			return false;
		}
			DMA_IFCR |= (1 << 1);//Clear all interrupt flag
		return true;
	}
	
	
//------Analog value of Torque Sensor
volatile float v_offset = 1.318f;	//V
volatile float Sens = 0.0024f;		//V - Sensitivity
volatile float torque_Nm;
	
//------Status of Hub VESC-----
volatile float ERPM_H;
volatile float Current_H;
volatile float Duty_Cycle_H;
volatile float Current_In_H;
volatile float Volts_In_H;
	
volatile float RPM_H;
volatile float Speed_Kpm;
	
//-----Status of Load VESC-----
volatile float ERPM_L;
volatile float Current_L;
volatile float Duty_Cycle_L;
volatile float Current_In_L;
volatile float Volts_In_L;
	
//-----Telemetry------
typedef enum{
	Load_level,
	Torque_Nm,
	
	Hub_ERPM,
	Hub_Speed,
	Hub_Motor_Current,
	Hub_DutyCycle,
	Hub_Current_In,
	Hub_Voltage_In,
	
	Load_ERPM,
	Load_Motor_Current,
	Load_DutyCycle,
	Load_Current_In,
	Load_Voltage_In,
	
	Buffer_Count
}Tele_Field;
static float Buffer_data[Buffer_Count];
static uint8_t tx_buffer[2 + Buffer_Count*sizeof(float) + 1]; 

static void Telemetry_Send(void){		
		//Encode Transmitt frame 
		Buffer_data[Load_level] = (float)Num_Load_Level;
		Buffer_data[Torque_Nm] = (float)torque_Nm;
		Buffer_data[Hub_ERPM] = (float)Status_Hub.Status_1.ERPM;//Hub_ERPM
		Buffer_data[Hub_Speed] = (float)Speed_Kpm;//Hub_Speed
		Buffer_data[Hub_Motor_Current] = (float)Status_Hub.Status_1.Current;//Hub_Motor_Current
		Buffer_data[Hub_DutyCycle] = (float)Status_Hub.Status_1.Duty;//Hub_DutyCycle
		Buffer_data[Hub_Current_In] = (float)Status_Hub.Status_4.Current_In;//Hub_Current_In
		Buffer_data[Hub_Voltage_In] = (float)Status_Hub.Status_5.Volts_In;//Hub_Voltage_In
		Buffer_data[Load_ERPM] = (float)Status_Load.Status_1.ERPM;//Load_ERPM
		Buffer_data[Load_Motor_Current] = (float)Status_Load.Status_1.Current;//Load_Motor_Current
		Buffer_data[Load_DutyCycle] = (float)Status_Load.Status_1.Duty;//Load_DutyCycle
		Buffer_data[Load_Current_In] = (float)Status_Load.Status_4.Current_In;//Load_Current_In
		Buffer_data[Load_Voltage_In] = (float)Status_Load.Status_5.Volts_In;
		
		tx_buffer[0] = Header_Frame_1;
		tx_buffer[1] = Header_Frame_2;
		tx_buffer[54] = End_Frame;
		memcpy(&tx_buffer[2], Buffer_data, sizeof(Buffer_data));
		DMA_USART1_Start(tx_buffer, sizeof(tx_buffer));
}

SemaphoreHandle_t xDMA_Uart_Semaphore; //Process encode data to transmit UART
QueueHandle_t can_rx_queue;//Process encode data from Receive CAN 


//-----CALL API-----//
char buffer[100];
void mPrintf(const char *format,...){
	for(uint8_t i = 0; i < sizeof(buffer); i++){
		buffer[i] = 0;
	}
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
	USART3_SendString((char *)buffer);
}

int main(){
	RCC_Config_8Mhz();//Enable Sysclock HSE is 8MHz
	RCC_Enable_PortA();
	RCC_Enable_PortB();
	RCC_Enable_AFIO();
	RCC_Enable_ADC1();
	RCC_Enable_DMA();
	RCC_Enable_Tim2();
	RCC_Enable_CAN1();
	RCC_Enable_USART1();
	RCC_Enable_USART3();
	
void Task_User(void *parameter);
void Task_Decode(void *parameter);
void Task_Encode(void *parameter);
	//USART2 - Debug Task
	
	
	//DMA-ADC - Get signal data  
	GPIO_Config(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT_ANALOG);
	DMA_ADC1_Init(adc_value, ADC_SAMPLE_TIME, 1);//1: Circular mode
	ADC1_CH0_Init();
		
	//GPIO-Load level
	GPIO_Config(GPIOA, GPIO_PIN_1, GPIO_MODE_INPUT_PU);//Low load
	GPIO_Config(GPIOA, GPIO_PIN_2, GPIO_MODE_INPUT_PU);//Medium load
	GPIO_Config(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT_PU);//High load
	
	//DMA-UART
	USART1_Init(115200);
	DMA_USART1_Init();
	
	USART3_Init(9600);
	
	
	xDMA_Uart_Semaphore = xSemaphoreCreateBinary();
	can_rx_queue = xQueueCreate(16, sizeof(CAN1_Frame_Type));
	
	if(xTaskCreate(Task_User, "User", 256, NULL, 3, NULL) != pdPASS ||
		xTaskCreate(Task_Decode, "Receive_Decode", 256, NULL, 2, NULL) != pdPASS||
		xTaskCreate(Task_Encode, "Encode_Transmit", 256, NULL, 2, NULL) != pdPASS) {
		mPrintf("Tasks are not creat\n");
			while(1){}
		}
	
	if(can_rx_queue == NULL){
		mPrintf("CAN_RX_Queue is not creat\n");
	}
	
	if(xDMA_Uart_Semaphore == NULL){
		mPrintf("Semaphore is not creat\n");
		while(1){}
	}
	
	xSemaphoreGive(xDMA_Uart_Semaphore); //Release Semaphore to Task encode runs
	vTaskStartScheduler();
	
	while(1){}
	return 0;
}

//-----Task-----
void Task_User(void *parameter){
	
	mPrintf("Task User is running\n");
	while(1){
		if(GPIO_Read_Pin(GPIOA, GPIO_PIN_1) == 0){
			Num_Load_Level = 1;
			VESC_SET_CURRENT_BRAKE(Low_Load);
			//mPrintf("Low load\n");
		}
		else if(GPIO_Read_Pin(GPIOA, GPIO_PIN_2) == 0){
			Num_Load_Level = 2;
			VESC_SET_CURRENT_BRAKE(Medium_Load);
			//mPrintf("Medium load\n");
		}
		else if(GPIO_Read_Pin(GPIOA, GPIO_PIN_3) == 0){
			Num_Load_Level = 3;
			VESC_SET_CURRENT_BRAKE(High_Load);
			//mPrintf("High load\n");
		}
		else{
			Num_Load_Level = 0;
			VESC_SET_CURRENT_BRAKE(No_Load);
			//mPrintf("None load\n");
		}
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

volatile uint32_t can_processed_count = 0;
void Task_Decode(void *parameter){
	
	CAN1_Frame_Type frame;
	mPrintf("Task Decode is running\n");
	
	CAN1_Init(500000, 1);
	
	while(1){
		
		if(xQueueReceive(can_rx_queue, &frame, pdMS_TO_TICKS(1)) == pdTRUE){
			CAN1_Process_Frame(&frame);
			can_processed_count++;
		}
		
		
		if(DMA_ADC1_Process()){
			uint32_t sum = 0;
			for(uint8_t i = 0; i < ADC_SAMPLE_TIME; i++){
				sum += adc_value[i];
			}
			float adc_avg = (float)sum/ADC_SAMPLE_TIME;
			float v_measure = (adc_avg * 3.3f)/4095;
			float raw_torque_Nm = (v_measure - v_offset)/Sens;
			
			if((raw_torque_Nm < 30.0f) && (raw_torque_Nm > -30.0f)){
				torque_Nm = raw_torque_Nm;
			}
			else if((raw_torque_Nm > 30.0f)){
				torque_Nm = 30.0f;
			}
			else if((raw_torque_Nm < -30.0f)){
				torque_Nm = -30.0f;
			}
			//Scale minimum value to 0Nm
			if((raw_torque_Nm < Torque_DEADBAND) && (raw_torque_Nm > - Torque_DEADBAND)){
				torque_Nm = 0.0f;
				mPrintf("Torque Nm = %.2f", torque_Nm);
			}
		}
		
		
		//CAN receive - Decode 
		ERPM_H = Status_Hub.Status_1.ERPM / 1.0f;
		Current_H = Status_Hub.Status_1.Current / 10.0f;
		Duty_Cycle_H = Status_Hub.Status_1.Duty / 1000.0f;
		Current_In_H = Status_Hub.Status_4.Current_In / 10.0f;
		Volts_In_H = Status_Hub.Status_5.Volts_In / 10.0f;
		
		ERPM_L = Status_Load.Status_1.ERPM / 1.0f;
		Current_L = Status_Load.Status_1.Current / 10.0f;
		Duty_Cycle_L = Status_Load.Status_1.Duty / 1000.0f;
		Current_In_L = Status_Load.Status_4.Current_In / 10.0f;
		Volts_In_L = Status_Load.Status_5.Volts_In / 10.0f;
		
		RPM_H = ERPM_H / H_Poles_Pair;
		Speed_Kpm = RPM_H*(2*3.14f*R_Tire/1000.0f) * 60/1000;
		
	}
}

void Task_Encode(void *parameter){
	mPrintf("Task Encode is running\n");;
	while(1){
		xSemaphoreTake(xDMA_Uart_Semaphore, portMAX_DELAY);
		
		Telemetry_Send();
		vTaskDelay(pdMS_TO_TICKS(100));
	}
	
}
