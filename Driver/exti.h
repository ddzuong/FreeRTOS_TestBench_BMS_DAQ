#ifndef __EXTI_H
#define __EXTI_H
#include "TYPE.h"
#include "afio.h"
#include "gpio.h"

#define EXTI_RISING_MODE		0x00
#define EXTI_FALLING_MODE		0x01
#define EXTI_BOTH_MODE			0x02

#define NVIC_ISER0		*((volatile uint32_t *)0xE000E100UL) //Core-Peripheral 
#define NVIC_ISER1		*((volatile uint32_t *)0xE000E104UL)

typedef struct{
	__32BIT IMR;
	__32BIT EMR;
	__32BIT RTSR;
	__32BIT FTSR;
	__32BIT SWIER;
	__32BIT PR;
}EXTI_TypeDef;

#define EXTI		((volatile EXTI_TypeDef *)0x40010400UL)

void EXTI0_IRQHandler(void);
uint8_t get_pin_Number(uint16_t gpio_pin);
void EXTI_Init(uint16_t gpio_pin,volatile GPIO_TypeDef *Port, uint8_t type);
void NVIC_USART1_En(void);
void NVIC_DMA_USART_En(void);
#endif