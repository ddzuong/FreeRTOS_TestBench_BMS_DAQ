#include "dma.h"
#include <stdint.h>
#include "rcc.h"
#include "adc.h"
#include "exti.h"
#include "uart.h"
#include <stdio.h>
#include <stddef.h>
void DMA_Init(void){
	RCC_Enable_DMA();
}

void DMA_ADC1_Init(volatile uint16_t *buffer, uint16_t len, uint8_t mode){
	DMA_CCR1 &= ~(uint32_t)(1 << 0); //Disable channel 1 to config 
	if(mode == 1){
		DMA_CCR1 |= (1 << 5); //Circular mode
	}
	else{
		DMA_CCR1 &= ~(uint32_t)(1 << 5); //Normal mode
	}
	DMA_CCR1 &= ~(uint32_t)(1 << 4); //Read Data from Peripheral
	DMA_CCR1 |= (1 << 7);//Enable Memory increment
	DMA_CCR1 &= ~(uint32_t)(1 << 6);//Disable Peripheral increment 
	DMA_CCR1 |= (1 << 8); //PeriPheral size = 16bit
	DMA_CCR1 |= (1 << 10); //Memory size = 16bit 
	DMA_CCR1 |= (0x03 << 12); //Very high priority
	
	DMA_CNDTR1 = len;//DMA get length samples 
	
	DMA_CPAR1 = (uint32_t)&ADC1_DR; //Address of Data_ADC1 
	DMA_CMAR1 = (uint32_t)(buffer);
	
	DMA_CCR1 |= (1 << 0); //Enable channel 1
}


void DMA_USART1_Init(void){
	DMA_CCR4 &= ~(uint32_t)(1 << 0);//Disable channel 4
	
	DMA_CCR4 = 0;
	DMA_IFCR |= (1 << 12);//Clear Interrupt flag
	
	DMA_CCR4 |= (1 << 3);//Transfer error interrupt enable
	DMA_CCR4 |= (1 << 4);//Read from Memory
	DMA_CCR4 &= ~(uint32_t)(1 << 5);// Normal mode
	DMA_CCR4 &= ~(uint32_t)(1 << 6);//Disable P increment
	DMA_CCR4 |= (1 << 7);// Enable Memory Increment
	DMA_CCR4 |= (0x02 << 12);//High priority
	DMA_CCR4 |= (1 << 1); //TCIE 

	//Turn on DMA on USART Peripheral
	USART1_CR3 |= (1 << 7); //DMA transmit to Peripheral
	
	DMA_CPAR4 = (uint32_t)&USART1_DR;	
	
	NVIC_DMA_USART_En();
}

static volatile bool dma_usart1_busy = false;
bool DMA_USART1_Start(const uint8_t *buffer, uint16_t len){
	if((buffer == NULL) || (len == 0)){
		return false;
	}
	
	//Can not config or change when DMA transmit
	if((dma_usart1_busy) || (DMA_CCR4 &(1 << 0)) != 0){
		return false;
	}
	
	//Clear interrupt flag
	DMA_IFCR = (1 << 12);
	
	//
	DMA_CMAR4 = (uint32_t)buffer;
	DMA_CNDTR4 = len;
	dma_usart1_busy = 1;
	DMA_CCR4 |= (1 << 0);//Enable channel 4
	
	return true;
}
void DMA1_Channel4_IRQHandler(void){
	//Check error 
	if(DMA_ISR & (1 << 15)){
		DMA_CCR4 &= ~(1 << 0);//Disable channel 4
		DMA_IFCR = (1 << 12);//Clear IT flag
		dma_usart1_busy = false;
	}
	
	//Complete transmit
	if(DMA_ISR &(1 << 13)){
		DMA_CCR4 &= ~(uint32_t)(1 << 0);//Disable channle 4
		DMA_IFCR = (1 << 12);
		dma_usart1_busy = false;
	}
}