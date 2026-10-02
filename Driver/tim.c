#include "tim.h"
#include "exti.h"

//PSC, ARR, CNT

uint8_t blink_status = 0; //LED-OFF

//Timer Interupprt
void TIM2_Init_IT(void){
	TIM2_PSC = 7999;
	TIM2_ARR = 999;
	TIM2_DIER |= (1 << 0); //Enable Interrupt
	TIM2_CR1 |= (1 << 0);
	NVIC_ISER0 |= (1 << 28); //Enable TIM2_NVIC Controller
}


void TIM2_IRQHandler(void){
	if(TIM2_SR & 0x01){
	//doing sth
		if(blink_status == 1){
			blink_status = 0;
		}
		else{
			blink_status = 1;
		}
		TIM2_SR &= ~((uint32_t)(1 << 0));
	}
}

void delay_ms(uint32_t ms){
	TIM2_PSC = 7999;
	TIM2_ARR = ms;
	TIM2_CNT = 0;
	TIM2_CR1 |= (1 << 0); //Set counter enable
	while(TIM2_CNT < ms){}
		TIM2_CR1 &= ~(uint32_t)(1 << 0); //Clear enable counter
		TIM2_CNT = 0;
}
	