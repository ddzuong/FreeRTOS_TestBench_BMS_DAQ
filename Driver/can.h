#ifndef __CAN_H
#define __CAN_H
#include <stdint.h>
#include <stdbool.h>

//--------------------BASE_ADRESS----------------
#define CAN1_ADD_BASE			0x40006400UL


//--------------------REGISTER_ADDRESS-----------
#define CAN1_MCR_OFFSET          0x000
#define CAN1_MSR_OFFSET          0x004
#define CAN1_TSR_OFFSET          0x008
#define CAN1_RF0R_OFFSET         0x00C
#define CAN1_RF1R_OFFSET         0x010
#define CAN1_IER_OFFSET          0x014              
#define CAN1_ESR_OFFSET          0x018
#define CAN1_BTR_OFFSET          0x01C
#define CAN1_TI0R_OFFSET         0x180
#define CAN1_TDT0R_OFFSET        0x184
#define CAN1_TDL0R_OFFSET        0x188
#define CAN1_TDH0R_OFFSET        0x18C
#define CAN1_TI1R_OFFSET         0x190
#define CAN1_TDT1R_OFFSET        0x194
#define CAN1_TDL1R_OFFSET        0x198
#define CAN1_TDH1R_OFFSET        0x19C
#define CAN1_TI2R_OFFSET         0x1A0
#define CAN1_TDT2R_OFFSET        0x1A4
#define CAN1_TDL2R_OFFSET        0x1A8
#define CAN1_TDH2R_OFFSET        0x1AC
#define CAN1_RI0R_OFFSET         0x1B0
#define CAN1_RDT0R_OFFSET        0x1B4
#define CAN1_RDL0R_OFFSET        0x1B8
#define CAN1_RDH0R_OFFSET        0x1BC
#define CAN1_RI1R_OFFSET         0x1C0
#define CAN1_RDT1R_OFFSET        0x1C4
#define CAN1_RDL1R_OFFSET        0x1C8
#define CAN1_RDH1R_OFFSET        0x1CC
#define CAN1_FMR_OFFSET          0x200
#define CAN1_FM1R_OFFSET         0x204
#define CAN1_FS1R_OFFSET         0x20C
#define CAN1_FFA1R_OFFSET        0x214
#define CAN1_FA1R_OFFSET         0x21C
#define CAN1_F0R1_OFFSET         0x240              
#define CAN1_F0R2_OFFSET         0x244
#define CAN1_F1R1_OFFSET         0x248
#define CAN1_F1R2_OFFSET         0x24C
#define CAN1_F2R1_OFFSET         0x250
#define CAN1_F2R2_OFFSET         0x254
#define CAN1_F3R1_OFFSET         0x258
#define CAN1_F3R2_OFFSET         0x25C
#define CAN1_F4R1_OFFSET         0x260
#define CAN1_F4R2_OFFSET         0x264
#define CAN1_F5R1_OFFSET         0x268
#define CAN1_F5R2_OFFSET         0x26C
#define CAN1_F6R1_OFFSET         0x270
#define CAN1_F6R2_OFFSET         0x274
#define CAN1_F7R1_OFFSET         0x278
#define CAN1_F7R2_OFFSET         0x27C
#define CAN1_F8R1_OFFSET         0x280
#define CAN1_F8R2_OFFSET         0x284
#define CAN1_F9R1_OFFSET         0x288
#define CAN1_F9R2_OFFSET         0x28C
#define CAN1_F10R1_OFFSET        0x290
#define CAN1_F10R2_OFFSET        0x294
#define CAN1_F11R1_OFFSET        0x298
#define CAN1_F11R2_OFFSET        0x29C
#define CAN1_F12R1_OFFSET        0x2A0
#define CAN1_F12R2_OFFSET        0x2A4
#define CAN1_F13R1_OFFSET        0x2A8
#define CAN1_F13R2_OFFSET        0x2AC

//--------------------CAN-------------------
#define CAN1_MCR                  *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_MCR_OFFSET  ))
#define CAN1_MSR                  *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_MSR_OFFSET  ))
#define CAN1_TSR                  *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TSR_OFFSET  ))
#define CAN1_RF0R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RF0R_OFFSET ))
#define CAN1_RF1R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RF1R_OFFSET ))
#define CAN1_IER                  *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_IER_OFFSET  ))               
#define CAN1_ESR                  *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_ESR_OFFSET  ))
#define CAN1_BTR      		        *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_BTR_OFFSET  ))
#define CAN1_TI0R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TI0R_OFFSET ))
#define CAN1_TDT0R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TDT0R_OFFSET))
#define CAN1_TDL0R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TDL0R_OFFSET))
#define CAN1_TDH0R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TDH0R_OFFSET))
#define CAN1_TI1R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TI1R_OFFSET ))
#define CAN1_TDT1R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TDT1R_OFFSET))
#define CAN1_TDL1R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TDL1R_OFFSET))
#define CAN1_TDH1R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TDH1R_OFFSET))
#define CAN1_TI2R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TI2R_OFFSET ))
#define CAN1_TDT2R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TDT2R_OFFSET))
#define CAN1_TDL2R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TDL2R_OFFSET))
#define CAN1_TDH2R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_TDH2R_OFFSET))
#define CAN1_RI0R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RI0R_OFFSET ))
#define CAN1_RDT0R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RDT0R_OFFSET))
#define CAN1_RDL0R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RDL0R_OFFSET))
#define CAN1_RDH0R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RDH0R_OFFSET))
#define CAN1_RI1R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RI1R_OFFSET ))
#define CAN1_RDT1R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RDT1R_OFFSET))
#define CAN1_RDL1R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RDL1R_OFFSET))
#define CAN1_RDH1R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_RDH1R_OFFSET))
#define CAN1_FMR                  *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_FMR_OFFSET  ))
#define CAN1_FM1R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_FM1R_OFFSET ))
#define CAN1_FS1R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_FS1R_OFFSET ))
#define CAN1_FFA1R                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_FFA1R_OFFSET))
#define CAN1_FA1R                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_FA1R_OFFSET ))
#define CAN1_F0R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F0R1_OFFSET ))
#define CAN1_F0R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F0R2_OFFSET ))
#define CAN1_F1R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F1R1_OFFSET ))
#define CAN1_F1R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F1R2_OFFSET ))
#define CAN1_F2R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F2R1_OFFSET ))
#define CAN1_F2R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F2R2_OFFSET ))
#define CAN1_F3R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F3R1_OFFSET ))
#define CAN1_F3R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F3R2_OFFSET ))
#define CAN1_F4R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F4R1_OFFSET ))
#define CAN1_F4R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F4R2_OFFSET ))
#define CAN1_F5R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F5R1_OFFSET ))
#define CAN1_F5R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F5R2_OFFSET ))
#define CAN1_F6R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F6R1_OFFSET ))
#define CAN1_F6R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F6R2_OFFSET ))
#define CAN1_F7R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F7R1_OFFSET ))
#define CAN1_F7R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F7R2_OFFSET ))
#define CAN1_F8R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F8R1_OFFSET ))
#define CAN1_F8R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F8R2_OFFSET ))
#define CAN1_F9R1                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F9R1_OFFSET ))
#define CAN1_F9R2                 *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F9R2_OFFSET ))
#define CAN1_F10R1                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F10R1_OFFSET))
#define CAN1_F10R2                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F10R2_OFFSET))
#define CAN1_F11R1                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F11R1_OFFSET))
#define CAN1_F11R2                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F11R2_OFFSET))
#define CAN1_F12R1                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F12R1_OFFSET))
#define CAN1_F12R2                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F12R2_OFFSET))
#define CAN1_F13R1                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F13R1_OFFSET))
#define CAN1_F13R2                *((volatile uint32_t *)(CAN1_ADD_BASE + CAN1_F13R2_OFFSET))



//---------------------------NVIC Core--------------------------
#define NVIC_ISER0		*((volatile uint32_t *)0xE000E100UL) //Core-Peripheral 

//---------------------------DATA_TYPE--------------------------

//Tx Frame
typedef struct{
	uint32_t ID; //Standard ID(11bit) or Extended ID(29bit)
	uint8_t	IDE; //Mode standard or Extended ID?
	uint8_t RTR; //Mode Data frame or Remote frame?
	uint8_t DLC; //Length of data (unit: Byte)
	uint8_t data[8]; //Data byte
}CAN1_Frame_Type;

//Rx Frame 
typedef struct{
	uint32_t id;
	uint8_t ide;
	uint8_t rtr;
	uint8_t dlc;
	uint8_t data[8];
}CAN1_RxFrame_Type;

//Status Commands
typedef struct{
	struct{
		int32_t ERPM;
		int16_t Current;
		int16_t Duty;
	}Status_1;
	
	struct{
		int32_t Amp_H;
		int32_t Amp_H_Chg;
	}Status_2;
	
	struct{
		int32_t WATT_H;
		int32_t WATT_H_CHG;
	}Status_3;
	
	struct{
		int16_t Temp_FET;
		int16_t Temp_Motor;
		int16_t Current_In;
		int16_t Pid_Pos;
	}Status_4;
	
	struct{
		int32_t Tachometer;
		int16_t Volts_In;
	}Status_5;
	
}Status_Command;

extern volatile Status_Command Status_Hub;
extern volatile Status_Command Status_Load;

//Single-Frame (simple) Commands
typedef enum{
	CAN_PACKET_SET_DUTY										= 0,
	CAN_PACKET_SET_CURRENT								= 1,
	CAN_PACKET_SET_CURRENT_BRAKE					= 2,
	CAN_PACKET_SET_RPM										= 3,
	CAN_PACKET_SET_POS										= 4,
	CAN_PACKET_SET_CURRENT_REL						= 10,
	CAN_PACKET_SET_CURRENT_BRAKE_REL			= 11,
	CAN_PACKET_SET_CURRENT_HANDBRAKE			= 12,
	CAN_PACKET_SET_CURRENT_HANDBRAKE_REL	= 13
}Command_ID;


//---------------------------API--------------------------------
void CAN1_Init(uint32_t Baudrate, uint8_t Prescaler);
uint8_t CAN1_Send(const CAN1_Frame_Type *Frame);
uint8_t CAN1_MessagePending(void);
uint8_t CAN1_Receive(CAN1_Frame_Type *Frame);
void CAN1_Process_Frame(const CAN1_Frame_Type *Frame); //Only read, must not change data frame
void USB_LP_CAN1_RX0_IRQHandler(void);
void CAN1_RX1_IRQHandler(void);
void CAN1_SCE_IRQHandler(void);
void NVIC_CAN1_En(void);
void VESC_SET_CURRENT_BRAKE(float current);
#endif