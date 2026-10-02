#include <stdint.h>
#include <stdio.h>
#include "can.h"
#include "rcc.h"
#include "gpio.h"
#include "exti.h"

#define ID_node_Hub		101U
#define ID_node_Load	67U

volatile Status_Command Status_Hub;
volatile Status_Command Status_Load;

void CAN1_Init(uint32_t Baudrate, uint8_t Prescaler){
	if((Baudrate != 500000U) || (Prescaler != 1U)){
		return;
	}
	RCC_Enable_PortA();
	RCC_Enable_AFIO();
	RCC_Enable_CAN1();
	//PA12: TxCAN, mode OUTPUT Pushpull
	GPIO_Config(GPIOA, GPIO_PIN_12, GPIO_MODE_OUTPUT_AF_PP);
	//PA11: RxCAN, mode INPUT
	GPIO_Config(GPIOA, GPIO_PIN_11, GPIO_MODE_INPUT_FLOATING);
	CAN1_MCR &= ~(uint32_t)(1 << 1); //Disable sleep mode
	while(CAN1_MSR & (1 << 1)){}; //Wait for leaving sleep mode
	CAN1_MCR |= (1 << 0); //Initialization mode
	while(!(CAN1_MSR & (1 << 0))){} //Wait for Initialization mode
	
	CAN1_MCR |= (1 << 0); //Initialization mode
	CAN1_MCR |= (1 << 2); //Enable Transmit FIFO priority
	CAN1_MCR &= ~(uint32_t)(1 << 3); //Disable RFLM
	CAN1_MCR &= ~(uint32_t)(1 << 4); //Automatic retransmission
	
	/*
	CAN BIT TIMING
	Do tre lan truyen: t_PRO_SEG = 2 * (t_bus + t_transceiver)
	Thoi gian 1 bit: NBT = 1 / Baudrate
	Thoi gian 1 chu ki xung nhip CAN: t_CANCLK = 1 / f_clk
	Chon prescaler: tq = BRP * t_CANCLK
	Tong so TQ trong 1 bit: NBT_TQ = NBT/tq
	
	Chuyen do tre sang TQ: PROP_SEG_TQ = CEIL(t_PROP_SEG/tq)
	
	Cau truc co ban cua 1 bit: Total TQ = SYN_SEG + PROP_SEG + PHASE_SEG1 + PHASE_SEG2
	
	Dat Sample pointer o 81.25%
		
		STM32F103C8T6: F_CLK su dung HSE = 8MHz --> f_CLK = 8MHz
		Chon Baudrare = 500kbps
		t_PRO_SEG = 2*(120 + 0.25 + 50) = 340ns 
		Nominal Bit Time: NBT = 1/500000 = 2us
		CAN clock frequency (CAN_CLK) = F_CLK/Prescaler = 8/1 = 8MHz
		Time quanta: TQ = 1/(CAN_CLK/Prescaler) = 1/8 = 125 ns
		
		Thoi gian 1 chu ki xung nhip can t_CANCLK = 1/F_CLK = 1/8000000 = 125ns
		tq = BRP*t_CANCLK = 1*125 = 125ns
		total TQ trong 1 bit: NBT_TQ = NBT/tq = 2000ns/125ns = 16	TQ
		PROP_SEG_TQ = CEIL(t_PROP_SEG/tQ) = CEIL(340s/125ns) =  3 TQ
		NBT_TQ = SYN_SEG + PROP_SEG + PHASE_SEG1 + PHASE_SEG2
		STM32F103CBT6 gop chung PROP_SEG va BS1
		16 = 1 + PHASE_SEG1 + PHASE_SEG2 --> BS1 + BS2 = 15
		Sample point at 81.25% --> 13 = 1 + (BS1) --> BS1 = 12 --> BS2 = 3
		SJW = min(4, BS1) = 4
		tolerant deltaf/f <= SJW/(20*NBT_TQ) = 4/(20*16)= 0.0125 = 1.25%
		
		Check again:
		Total_TQ = SYN_SEG +PROP_SEG + PS1 + PS2
		Bitrare = PCLK1/(BRP * Total_TQ)
		Sample point = (1 + BS1)/Total_TQ
	*/
	CAN1_BTR |= (0x00 << 0); //Prescaler - 1 = 1 - 1 = 0
	CAN1_BTR |= (0x0B << 16); //Phase_Seg1 - 1= 12.	 TS1 - 1
	CAN1_BTR |= (0x02 << 20); //Phase_Seg2 - 1= 3. 	 TS2 -1		
	CAN1_BTR |= (0x00 << 24); //SWJ_min(3, Phase_Seg1) - 1= 1. SJW - 1. stm32f1 chi co SJW min la 1
	//CAN1_BTR |= (0x01 << 30); //Test Loop back mode
	//CAN FILTER
	CAN1_FMR |= (1 << 0); //Enable Fix Filter registers (FINIT)
	CAN1_FA1R &= ~(uint32_t)(1 << 0); //Disable bank 0 when change config
	CAN1_FM1R &= ~(uint32_t)(1 << 0); //Identifier Mask mode
	CAN1_FS1R |= (1 << 0); //Single Filter 32-bit
	CAN1_FFA1R &= ~(uint32_t)(1 << 0); //Choose FIFO0
	CAN1_F0R1 = 0; //Filter ID
	CAN1_F0R2 = 0; //Mask = 0,accept all node
	CAN1_FA1R |= (1 << 0); //Enale bank 0
	CAN1_FMR &= ~(uint32_t)(1 << 0); //End of Init
	
	CAN1_MCR &= ~(uint32_t)(1 << 0);// INRQ = 0
	while ((CAN1_MSR & (1 << 0))){}; //wait for Remove Initialization mode, CAN enable

	CAN1_IER |= (1 << 1)|(1 << 8) | (1 << 9) | (1 << 10) | (1 << 11) | (1 << 15);
	NVIC_CAN1_En();
}


uint8_t CAN1_Send(const CAN1_Frame_Type *Frame){
	//Data frame: ID	|		IDE		|		RTR		|		DLC		|		DATA		|		
	//CAN1_TSR: Check Mailbox empty
	//CAN1_TI0R: Write ID, IDE, RTR, TXRQ
	//CAN1_TDT0R: Write DLC
	//CAN1_TDL0R: Contain databyte 0 - 3
	//CAN1_TDH0R: Contain databyte 4 - 7
	
	uint32_t transmit_id = 0;//Transmit_identifier as CAN_TIxR
	//Check Frame
	if((Frame == NULL) || (Frame->DLC > 8)){
		return 0;
	}
	
	//TME0 empty
	if((CAN1_TSR &(1 << 26)) == 0){
		return 0;
	}
	
	//Prepare ID, IDE
	if(Frame->IDE == 0){
		if(Frame->ID > 0x7FF){
			return 0;
		}
		transmit_id = ((Frame->ID & 0x7FF) << 21);
		//CAN1_TI0R &= ~(uint8_t)(1 << 2); //IDE mode = 0
	}
	else if(Frame->IDE == 1){
		if(Frame->ID > (0x1FFFFFFF)){
			return 0;
		}
		transmit_id = ((Frame->ID & 0x1FFFFFFF) << 3) | (1 << 2);
		//CAN1_TI0R |= (1 << 2); //IDE mode = 1
	}
	else{
		return 0;
	}
	
		//Prepare RTR 
	if(Frame->RTR == 0){
		transmit_id &= ~(uint8_t)(1 << 1);
	}
	else if(Frame->RTR == 1){
		transmit_id |= (1 << 1);
	}
	
	//Prepare Data
	CAN1_TI0R = transmit_id;
	CAN1_TDT0R = Frame->DLC & 0x0F; //DLC = 4 bit
	CAN1_TDL0R = (((uint32_t)Frame->data[0] << 0) | ((uint32_t)Frame->data[1] << 8) | ((uint32_t)Frame->data[2] << 16) | ((uint32_t)Frame->data[3] << 24));
	CAN1_TDH0R = (((uint32_t)Frame->data[4] << 0) | ((uint32_t)Frame->data[5] << 8) | ((uint32_t)Frame->data[6] << 16) | ((uint32_t)Frame->data[7] << 24));
	
	CAN1_TI0R |= (1 << 0); //TXRQ
	
	return 1;
}

uint8_t CAN1_MessagePending(void){
	//Check how many messages are pending in FIFO0
		return (uint8_t)(CAN1_RF0R &(0x03));
}

uint8_t CAN1_Receive(CAN1_Frame_Type *Frame){
	//RF0R -->Check FMP and RF0M0
	//RI0R -->Read ID, IDE, RTR
	//RDT0R -->Read DLC
	//RDL0R -->Read data byte 0-3
	//RDHOR -->Read data byte 4-7
	uint32_t receive_id;
	uint32_t receive_dlc;
	uint32_t receive_data_l;
	uint32_t receive_data_h;
	
	if(Frame == NULL){
		return 0;
	}
	
	//FIFO0 not have frame 
	if(!(CAN1_RF0R & 0x03)){
		return 0;
	}
	
	//Read mailbox after release FIFO
	receive_id 	= CAN1_RI0R;
	receive_dlc	= CAN1_RDT0R;
	receive_data_l		= CAN1_RDL0R;
	receive_data_h		= CAN1_RDH0R;
	
	//Read ID, IDR and RTR
	Frame->IDE = (uint8_t)(receive_id >> 2) & 0x01;
	Frame->RTR = (uint8_t)(receive_id >> 1) & 0x01;
	
	//Decode Standard va Extended ID
	if(Frame->IDE == 0){
		Frame->ID = (receive_id >> 21) & 0x7FF;
	}
	else if(Frame->IDE == 1){
		Frame->ID	= (receive_id >> 3) & 0x1FFFFFFF;
	}
	
	//Read DLC 
	Frame->DLC = (uint8_t)(receive_dlc & 0x0F);
	
	if(Frame->DLC > 8){
		Frame->DLC = 8;
	}
	
	//Encode Data
	Frame->data[0] = (uint8_t)(receive_data_l >> 0);
	Frame->data[1] = (uint8_t)(receive_data_l >> 8);
	Frame->data[2] = (uint8_t)(receive_data_l >> 16);
	Frame->data[3] = (uint8_t)(receive_data_l >> 24);
	Frame->data[4] = (uint8_t)(receive_data_h >> 0);
	Frame->data[5] = (uint8_t)(receive_data_h >> 8);
	Frame->data[6] = (uint8_t)(receive_data_h >> 16);
	Frame->data[7] = (uint8_t)(receive_data_h >> 24);
	//Release FIFO 
	CAN1_RF0R |= (1 << 5);//Release FIFO0 output mailbox
	
	return 1;
}

void CAN1_Process_Frame(const CAN1_Frame_Type *Frame){
	//Command ID 
	uint16_t command_id;
	uint16_t controller_id;
	volatile Status_Command *target;
	//Check frame on bus
	if((Frame == NULL) || (Frame->IDE == 0)){
		return;
	}
	command_id = (uint16_t)(Frame->ID >> 8);//Status n
	controller_id = (uint16_t)(Frame->ID & 0xFF);//ID of Node 
	
	if(controller_id == ID_node_Hub){
		target = &Status_Hub;
	}
	else if(controller_id == ID_node_Load){
		target = &Status_Load;
	}
	else{
		return;
	}
	
	switch(command_id){
		case 9:
			target->Status_1.ERPM = ((Frame->data[0] << 24) | (Frame->data[1] << 16) | (Frame->data[2] << 8) | (Frame->data[3] << 0));
			target->Status_1.Duty = ((Frame->data[6] << 8) | (Frame->data[7] << 0));
			target->Status_1.Current = ((Frame->data[4] << 8) | (Frame->data[5] << 0));
		break;
		
		case 14:
			target->Status_2.Amp_H = ((Frame->data[0] << 24) | (Frame->data[1] << 16) | (Frame->data[2] << 8) | (Frame->data[3] << 0));
			target->Status_2.Amp_H_Chg = ((Frame->data[4] << 24) | (Frame->data[5] << 16) | (Frame->data[6] << 8) | (Frame->data[7] << 0));
		break;
		
		case 15:
			target->Status_3.WATT_H = ((Frame->data[0] << 24) | (Frame->data[1] << 16) | (Frame->data[2] << 8) | (Frame->data[3]));
			target->Status_3.WATT_H_CHG = ((Frame->data[4] << 24) | (Frame->data[5] << 16) | (Frame->data[6] << 8) | (Frame->data[7]));
		break;
		
		case 16:
			target->Status_4.Temp_FET = ((Frame->data[0] << 8) | (Frame->data[1] << 0));
			target->Status_4.Temp_Motor = ((Frame->data[2] << 8) | (Frame->data[3] << 0));
			target->Status_4.Current_In = ((Frame->data[4] << 8) | (Frame->data[5] << 0));
			target->Status_4.Pid_Pos = ((Frame->data[6] << 8) | (Frame->data[7] << 0));
		break;
		
		case 27:
			target->Status_5.Tachometer = ((Frame->data[0] << 24) | (Frame->data[1] << 16) | (Frame->data[2] << 8) | (Frame->data[3]));
			target->Status_5.Volts_In = ((Frame->data[4] << 8) | (Frame->data[5] << 0));
		break;
	}
		
//		//Status1-----Command ID: 9------ERPM, Current, Duty Cycle
//		uint32_t raw_erpm = ((Frame->data[0] << 24) | (Frame->data[1] << 16) | (Frame->data[2] << 8) | (Frame->data[3] << 0));
//		uint16_t raw_current = ((Frame->data[4] << 8) | (Frame->data[5] << 0));
//		uint16_t raw_duty = ((Frame->data[6] << 8) | (Frame->data[7] << 0));
//		
//		Status_Hub.Status_1.Data.ERPM	= raw_erpm;
//		Status_Hub.Status_1.Data.Current = raw_current;
//		Status_Hub.Status_1.Data.Duty	= raw_duty;
//		
//		Status_Load.Status_1.Data.ERPM = raw_erpm;
//		Status_Load.Status_1.Data.Current	= raw_current;
//		Status_Load.Status_1.Data.Duty	= raw_duty;
//		
//		
//		//Status2-----Command ID: 14------Ah used, Ah Charged
//		uint32_t raw_amp_hours = ((Frame->data[0] << 24) | (Frame->data[1] << 16) | (Frame->data[2] << 8) | (Frame->data[3] << 0));
//		uint32_t raw_amp_hours_chg = ((Frame->data[4] << 24) | (Frame->data[5] << 16) | (Frame->data[6] << 8) | (Frame->data[7] << 0));
//		
//		Status_Hub.Status_2.Data.Amp_H = raw_amp_hours;
//		Status_Hub.Status_2.Data.Amp_H_Chg	= raw_amp_hours_chg;
//		
//		Status_Load.Status_2.Data.Amp_H	= raw_amp_hours;
//		Status_Load.Status_2.Data.Amp_H_Chg	= raw_amp_hours_chg;
//		
//		
//		//Status3-----Command ID: 15------Watt Hours, Watt Hours Charged
//		uint32_t raw_watt_hour	= ((Frame->data[0] << 24) | (Frame->data[1] << 16) | (Frame->data[2] << 8) | (Frame->data[3]));
//		uint32_t raw_watt_hour_chg		= ((Frame->data[4] << 24) | (Frame->data[5] << 16) | (Frame->data[6] << 8) | (Frame->data[7]));
//		
//		Status_Hub.Status_3.Data.WATT_H = raw_watt_hour;
//		Status_Hub.Status_3.Data.WATT_H_CHG	= raw_watt_hour_chg;
//		
//		Status_Load.Status_3.Data.WATT_H	= raw_watt_hour;
//		Status_Load.Status_3.Data.WATT_H_CHG	= raw_watt_hour_chg;
//		
//		
//		//Status4-----Command ID: 16------Temp FET, tempt Motor, Current In, PID Pos
//		uint16_t raw_tempFet = ((Frame->data[0] << 8) | (Frame->data[1] << 0));
//		uint16_t raw_tempMotor = ((Frame->data[2] << 8) | (Frame->data[3] << 0));
//		uint16_t raw_currentIn = ((Frame->data[4] << 8) | (Frame->data[5] << 0));
//		uint16_t raw_pidPos = ((Frame->data[6] << 8) | (Frame->data[7] << 0));
//		
//		Status_Hub.Status_4.Data.Temp_FET = raw_tempFet;
//		Status_Hub.Status_4.Data.Temp_Motor = raw_tempMotor;
//		Status_Hub.Status_4.Data.Current_In = raw_currentIn;
//		Status_Hub.Status_4.Data.Pid_Pos	= raw_pidPos;
//		
//		Status_Load.Status_4.Data.Temp_FET = raw_tempFet;
//		Status_Load.Status_4.Data.Temp_Motor = raw_tempMotor;
//		Status_Load.Status_4.Data.Current_In = raw_currentIn;
//		Status_Load.Status_4.Data.Pid_Pos	= raw_pidPos;
//		
//		
//		//Status5-----Command ID: 27------Tachometer, Volts In
//		uint32_t raw_tachometer = ((Frame->data[0] << 24) | (Frame->data[1] << 16) | (Frame->data[2] << 8) | (Frame->data[3]));
//		uint16_t raw_volts_In = ((Frame->data[4] << 8) | (Frame->data[5] << 0));
//		
//		Status_Hub.Status_5.Data.Tachometer = raw_tachometer;
//		Status_Hub.Status_5.Data.Volts_In = raw_volts_In;
//		
//		Status_Load.Status_5.Data.Tachometer = raw_tachometer;
//		Status_Load.Status_5.Data.Volts_In = raw_volts_In;
} 

void USB_LP_CAN1_RX0_IRQHandler(void){
	CAN1_Frame_Type RxFrame;
	while((CAN1_RF0R & 0x03) != 0){
		if(CAN1_Receive(&RxFrame) != 0){
			CAN1_Process_Frame(&RxFrame);
		}
	}
}

void NVIC_CAN1_En(void){
	NVIC_ISER0 |= (1 << 20);
	NVIC_ISER0 |= (1 << 22);//Enable CAN1_SCE
}

//Ham check loi ngat

volatile uint32_t CAN1_LastESR;
volatile uint32_t CAN1_LastErr;
void CAN1_SCE_IRQHandler(void){
	
	uint32_t msr;
	uint32_t esr;
	
	msr = CAN1_MSR;
	esr = CAN1_ESR;
	
	//Turn on Error interrupt ERRI
	if((msr & (1 << 2))){
		CAN1_LastESR = esr;
		CAN1_LastErr |= ((uint8_t)(esr >> 4) & 0x07);
		CAN1_ESR &= ~(uint8_t)(0x07 << 4);
		CAN1_MSR &= ~(uint8_t)(1 << 2);
	}
} 

void VESC_SET_CURRENT_BRAKE(float current){
	CAN1_Frame_Type tx = {0};
	uint16_t exID =(uint16_t)((CAN_PACKET_SET_CURRENT_BRAKE << 8) | ID_node_Load);
	uint32_t current_mA = current * 1000.0f;
	uint32_t raw_current = (uint32_t)current_mA;
	
	tx.ID = exID;
	tx.IDE = 1;//Extend ID 
	tx.RTR = 0;//Data frame
	tx.DLC = 4;
	//Encode data
	tx.data[0] = raw_current >> 24;
	tx.data[1] = raw_current >> 16;
	tx.data[2] = raw_current >> 8;
	tx.data[3] = raw_current;
	
	CAN1_Send(&tx);
}
void CAN1_RX1_IRQHandler(void); //Ham nhan ngat 
