#include "exti.h"
#include "gpio.h"

//void EXTI0_Init(void){
//	
//	AFIO->EXTICR1.reg &= ~(uint32_t)(0xF << 0); //clear bit 1111
//	//AFIO->EXTICR1.reg |= (0x01 << 0); //Set bit 0001 for PB0
//	EXTI->IMR.BITS.BIT0 = 1;
//	//EXTI->EMR.BITS.BIT0	= 1;
//	EXTI->RTSR.BITS.BIT0 = 1;//Turn ON Raising trigger
//	EXTI->FTSR.BITS.BIT0 = 0;//Turn OFF Failing trigger
//	
//	NVIC_ISER0 |= (1 << 6); //Turn on NVIC for EXTI0
//}

//void EXTI0_IRQHandler(void){
//	if(EXTI->PR.BITS.BIT0){
//		//Doing sth
//		GPIOB->ODR.BITS.ODR2 = 1;
//		EXTI->PR.reg = (uint32_t)(1 << 0); //clear bit -- end of interrupt
//	}
//}

uint8_t get_pin_number(uint16_t gpio_pin){
	for(uint8_t i = 0; i < 16; i++){
		if(gpio_pin & i){
			return i; //return gpio_pin
		}
	}
	return 0xFF;
} 

void EXTI_Init(uint16_t gpio_pin,volatile GPIO_TypeDef *Port, uint8_t type){
	uint8_t port_code;
	uint8_t pin;
	if (Port == GPIOA){
		port_code = 0;
	}
	else if(Port == GPIOB){
		port_code = 1;
	}
	else if(Port == GPIOC){
		port_code = 2;
	}
	else{}return;
	pin = get_pin_number(gpio_pin);
	uint32_t shift = (pin % 4) * 4;
	
	if(pin < 4){
		AFIO->EXTICR1.reg &= ~(uint32_t)(0xF << shift);
		AFIO->EXTICR1.reg |= (port_code << shift);
	}
	else if(pin < 8){
		AFIO->EXTICR2.reg &= ~(uint32_t)(0xF << shift);
		AFIO->EXTICR2.reg |= (port_code << shift);
	}
	else if(pin < 12){
		AFIO->EXTICR3.reg &= ~(uint32_t)(0xF << shift);
		AFIO->EXTICR3.reg |= (port_code << shift);
	}
	else{
		AFIO->EXTICR4.reg &= ~(uint32_t)(0x0F << shift);
		AFIO->EXTICR4.reg |= (port_code << shift);
	}
	
	EXTI->IMR.reg |= (1 << pin);
	if(type == EXTI_RISING_MODE){
		EXTI->RTSR.reg |= (1 << pin);
		EXTI->FTSR.reg &= ~(uint32_t)(1 << pin);
	}
	else if(type == EXTI_FALLING_MODE){
		EXTI->FTSR.reg |= (1 << pin);
		EXTI->RTSR.reg &= ~(uint32_t)(1 << pin);
	}
	else {
		EXTI->RTSR.reg |= (1 << pin);
		EXTI->FTSR.reg |= (1 << pin);
	}
	if(pin <= 4){
		NVIC_ISER0 |= (1 << (6 + pin));
	}
	else if(pin <= 9){
		NVIC_ISER0 |= (1 << (23));
	}
	else if(pin <= 15){
		NVIC_ISER1 |= (1 << (40-32));
	}
}

void NVIC_USART1_En(void){
	NVIC_ISER1 |= (1 << (37 - 32));
}

void NVIC_DMA_USART_En(void){
	NVIC_ISER0 |= (1 << 14); //DMA_USART_Enable
}