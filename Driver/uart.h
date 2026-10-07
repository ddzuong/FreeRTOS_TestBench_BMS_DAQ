#ifndef __UART_H
#define __UART_H

#include <stdint.h>

//-------------------ADDRESS_BASE--------------
#define USART1_ADD_BASE		0x40013800UL
#define USART3_ADD_BASE		0x40004800UL

//-------------------ADRESS_REGISTER-----------
#define USART1_SR_OFFSET     0x00
#define USART1_DR_OFFSET     0x04
#define USART1_BRR_OFFSET    0x08
#define USART1_CR1_OFFSET    0x0C
#define USART1_CR2_OFFSET    0x10
#define USART1_CR3_OFFSET    0x14
#define USART1_GTPR_OFFSET   0x18

#define USART3_SR_OFFSET     0x00
#define USART3_DR_OFFSET     0x04
#define USART3_BRR_OFFSET    0x08
#define USART3_CR1_OFFSET    0x0C
#define USART3_CR2_OFFSET    0x10
#define USART3_CR3_OFFSET    0x14
#define USART3_GTPR_OFFSET   0x18

//-------------------UART1---------------------
#define USART1_SR    *(((volatile uint32_t *)(USART1_ADD_BASE + USART1_SR_OFFSET  )))
#define USART1_DR    *(((volatile uint32_t *)(USART1_ADD_BASE + USART1_DR_OFFSET  )))
#define USART1_BRR   *(((volatile uint32_t *)(USART1_ADD_BASE + USART1_BRR_OFFSET )))
#define USART1_CR1   *(((volatile uint32_t *)(USART1_ADD_BASE + USART1_CR1_OFFSET )))
#define USART1_CR2   *(((volatile uint32_t *)(USART1_ADD_BASE + USART1_CR2_OFFSET )))
#define USART1_CR3   *(((volatile uint32_t *)(USART1_ADD_BASE + USART1_CR3_OFFSET )))
#define USART1_GTPR  *(((volatile uint32_t *)(USART1_ADD_BASE + USART1_GTPR_OFFSET)))

//-------------------UART2---------------------
#define USART3_SR    *(((volatile uint32_t *)(USART3_ADD_BASE + USART3_SR_OFFSET  )))
#define USART3_DR    *(((volatile uint32_t *)(USART3_ADD_BASE + USART3_DR_OFFSET  )))
#define USART3_BRR   *(((volatile uint32_t *)(USART3_ADD_BASE + USART3_BRR_OFFSET )))
#define USART3_CR1   *(((volatile uint32_t *)(USART3_ADD_BASE + USART3_CR1_OFFSET )))
#define USART3_CR2   *(((volatile uint32_t *)(USART3_ADD_BASE + USART3_CR2_OFFSET )))
#define USART3_CR3   *(((volatile uint32_t *)(USART3_ADD_BASE + USART3_CR3_OFFSET )))
#define USART3_GTPR  *(((volatile uint32_t *)(USART3_ADD_BASE + USART3_GTPR_OFFSET)))
	
//-------------------API-----------------------
void USART1_Init(uint32_t Baudrate);
void USART1_SendChar(char c);
void USART1_SendString(const char *str);
void USART1_IRQHandler(void); //Process data's received

void USART3_Init(uint32_t Baudrate);
void USART3_SendChar(char c);
void USART3_SendString(const char *str);
//void USART2_IRQHandler(void); //Process data's received

#endif