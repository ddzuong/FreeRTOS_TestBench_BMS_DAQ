#ifndef __TIM_H
#define __TIM_H

#include <stdint.h>

#define TIM2_BASE_ADD		((0x40000000UL))
#define TIM3_BASE_ADD		((0x40000400UL))
#define TIM4_BASE_ADD		((0x40000800UL))

#define TIM2_CR1				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x00))
#define TIM2_CR2				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x04))
#define TIM2_SMCR				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x08))
#define TIM2_DIER				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x0C))
#define TIM2_SR					*((volatile uint32_t *)(TIM2_BASE_ADD + 0x10))
#define TIM2_EGR				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x14))
#define TIM2_CCMR1	    *((volatile uint32_t *)(TIM2_BASE_ADD + 0x18))
#define TIM2_CCMR2	   	*((volatile uint32_t *)(TIM2_BASE_ADD + 0x1C))
#define TIM2_CCER				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x20))
#define TIM2_CNT				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x24))
#define TIM2_PSC				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x28))
#define TIM2_ARR				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x2C))
#define TIM2_CCR1				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x34))
#define TIM2_CCR2				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x38))
#define TIM2_CCR3				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x3C))
#define TIM2_CCR4				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x40))
#define TIM2_DCR				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x48))
#define TIM2_DMAR				*((volatile uint32_t *)(TIM2_BASE_ADD + 0x4C))

//extern uint8_t blink_status;

void TIM2_Init_IT(void);
void TIM2_IRQHandler(void);
void delay_ms(uint32_t ms);
#endif