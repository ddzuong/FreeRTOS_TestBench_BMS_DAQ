#include "rcc.h"
#include <stdio.h>

//RCC_CONFIG
//RCC_PORTX
void RCC_Config_72Mhz(void){
	//Enable HSE
	RCC->CR.BITS.HSEON		= 1;
	while(!RCC->CR.BITS.HSERDY){}
	ACR->BITS.LATENCY			= 2; //48Mhz <= SYSCLK <= 72Mhz
	RCC->CFGR.BITS.PLLSRC = 1;
	RCC->CFGR.BITS.PLLMUL = 7; //0111 x9 PLL
	RCC->CR.BITS.PLLON		= 1; //PLL ON
	while(!RCC->CR.BITS.PLLRDY){} 
		
	//AHB
	RCC->CFGR.BITS.HPRE		= 0; // 1
	//APB1
	RCC->CFGR.BITS.PPRE1  = 4; //2
	//APB2
	RCC->CFGR.BITS.PPRE2	= 0; //1
	
	RCC->CFGR.BITS.SW			= 1; 
	while(RCC->CFGR.BITS.SWS != 1){}
}
void RCC_Config_8Mhz(void){
	//Enable HSE
	RCC->CR.BITS.HSEON 		= 1; 
	while(!RCC->CR.BITS.HSERDY){}
	ACR->BITS.LATENCY			= 0; //0Mhz < SYSCLK <= 24Mhz
	//AHB
	RCC->CFGR.BITS.HPRE		= 0; //AHB = 8MHZ
	//APB1
	RCC->CFGR.BITS.PPRE1	= 0; //APB1 = 8MHz
	//APB2
	RCC->CFGR.BITS.PPRE2	= 0 ;//APB2 = 8MHz
	
	RCC->CFGR.BITS.SW			= 1;
	while(RCC->CFGR.BITS.SW != 1){}
}



void RCC_Enable_PortA(void){
	RCC->APB2ENR.BITS.IOPAEN	= 1;
}

void RCC_Enable_PortB(void){
	//RCC->APB2ENR.BITS.IOPBEN	= 1;
	RCC->APB2ENR.reg	|= (1 << 3);
}

void RCC_Enable_PortC(void){
	RCC->APB2ENR.BITS.IOPCEN	= 1;
}

void RCC_Enable_Tim2(void){
	RCC->APB1ENR.BITS.TIM2EN	= 1;
}

void RCC_Enable_AFIO(void){
	RCC->APB2ENR.BITS.AFIOEN	= 1;
}

void RCC_Enable_ADC1(void){
	RCC->APB2ENR.BITS.ADC1EN	= 1;
}

void RCC_Enable_USART1(void){
	RCC->APB2ENR.BITS.USART1EN = 1;
}
//void RCC_Enable_ADC2(void){
//	RCC->APB2ENR.BITS.ADC2EN	= 1;
//}

void RCC_Enable_CAN1(void){
	RCC->APB1ENR.BITS.CAN1EN = 1;
}

void RCC_Enable_DMA(void){
	RCC->AHBENR.BITS.DMA1EN = 1;
}