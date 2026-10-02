#include <stdint.h>
#include "adc.h"
#include "tim.h"
#include "dma.h"
/*
1. Config ADC: SMPR-->SQR-->CR
2. Read status: SR
3. Read data: DR
*/
void ADC1_CH0_Init(void){
	ADC1_SMPR2 &= ~(uint32_t)(0x07 << 0); //clear sample time selection
	ADC1_SMPR2 |= (0x07 << 0); //240 cycles 
	ADC1_SQR3		= 0; //Only channel 0 is scanned
	ADC1_CR2		|= (1 << 1); //Set ADC continue
	ADC1_CR2		|= (1 << 0); //Set ADC ON
	delay_ms(1);
	ADC1_CR2		|= (1 << 0);
	
	//CALIBRATON
	ADC1_CR2		|= (1 << 3);//RESET CALIB
	 
	while(ADC1_CR2 & (1 << 3)){}//RESET CALIB INITIALIZED
		
	ADC1_CR2		|= (1 << 2);//SET CALIB
	while(ADC1_CR2 & (1 << 2)){}//SET CALIB INITIALIZED
	
	ADC1_CR2		|= (1 << 8);//Enable ADC call DMA request 
	DMA_CCR1		|= (1 << 0);//Enable DMA channel 1
	ADC1_CR2		|= (1 << 22);//Start Conver
}
uint16_t ADC1_CH0_Read(void){
	//Read status
	while(!(ADC1_SR & (1 << 1))){}
	return (uint16_t)(ADC1_DR & 0xFFFF);
		
}