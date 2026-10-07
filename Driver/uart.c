#include "uart.h"
#include "gpio.h"
#include <stdint.h>
#include "exti.h"
#include "dma.h"
#include "tim.h"
//----Macro-----//
#define F_CLK	8000000UL
//SR,CR, NVIC_USART, DR, BBR

volatile uint8_t USART1_DIV_Mantissa;
volatile uint8_t USART1_DIV_Fraction;
volatile float USART1_DIV_Baudrate;

volatile uint8_t USART3_DIV_Mantissa;
volatile uint8_t USART3_DIV_Fraction;
volatile float USART3_DIV_Baudrate;

//Interrupt Receive data
void USART1_IRQHandler(void){
	uint32_t status; //USART_SR
	uint8_t data; //USART_DR
	
	status = USART1_SR;
	data = USART1_DR;
	
	if (status & (1 << 5)){
		//Doing sth
		char c = (char)(data & (0xFF)); //Read from DR
		USART1_SendChar(c);
	}
}

void USART3_IRQHandler(void){
	uint32_t status;	
	uint8_t data;
	
	status = USART3_SR;
	data = USART3_DR;
	
	if(status & (1 << 7)){
		char c = (char)(data & (0xFF)); 
		USART3_SendChar(c);
	}
}

void USART1_Init(uint32_t Baudrate){
	//PA9: OUTPUT AF PP
	GPIO_Config(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT_AF_PP);
	//Baudrate 9600b/s //8MHZ
	//PA10: INPUT FLOATING
	GPIO_Config(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT_FLOATING);
	//USARTx_BRR = f_clk/(16*baudrate)
	if(Baudrate == 9600){
		USART1_DIV_Baudrate = (float)(F_CLK / (16.0f * Baudrate)); //52.08
		USART1_DIV_Mantissa = ((uint8_t)USART1_DIV_Baudrate); //52
		USART1_DIV_Fraction = (uint8_t)((USART1_DIV_Baudrate - USART1_DIV_Mantissa) * 16.0f + 0.5f);//0.08 --> 1
		
		USART1_BRR = (uint16_t)(USART1_DIV_Mantissa << 4 | USART1_DIV_Fraction);
		//USART1_BRR = 0x341;	//Baudrate: 9600
	}
	if(Baudrate == 115200){
		USART1_DIV_Baudrate = (float)(F_CLK / (16.0f * Baudrate));
		USART1_DIV_Mantissa = ((uint8_t)USART1_DIV_Baudrate);
		USART1_DIV_Fraction = (USART1_DIV_Baudrate - USART1_DIV_Mantissa) * 16.0f + 0.5f;
		
		USART1_BRR = (uint16_t)((USART1_DIV_Mantissa << 4) | (USART1_DIV_Fraction));
		//USART1_BRR = 0x45;	//Baudrate: 115200
	}
	USART1_CR1 |= (1 << 13);//USART Enable
	USART1_CR1 |= (1 << 3);	//Enable transmit
	USART1_CR1 |= (1 << 2); //Enable Receive
	
	USART1_CR1 |= (1 << 5);	//Enable RXNEIE interrupt
	USART1_CR3 |= (1 << 7); //Enable DMA transmitter
	DMA_CCR4 |= (1 << 0); //Enable DMA channel 4
	NVIC_USART1_En();
}

void USART1_SendChar(char c){
	while(!(USART1_SR & (1 << 7)));
	USART1_DR  = c;
}

void USART1_SendString(const char *str){
	while(*str){
	USART1_SendChar(*str++);
	}
}

//Config USART3
void USART3_Init(uint32_t Baudrate){
	GPIO_Config(GPIOB, GPIO_PIN_10, GPIO_MODE_OUTPUT_AF_PP);
	GPIO_Config(GPIOB, GPIO_PIN_11, GPIO_MODE_INPUT_FLOATING);
	
	if(Baudrate == 9600){
		USART3_DIV_Baudrate = (float)(F_CLK / (16.0f * Baudrate));
		USART3_DIV_Mantissa = (uint8_t)(USART3_DIV_Baudrate);
		USART3_DIV_Fraction = (uint8_t)((USART3_DIV_Baudrate - USART3_DIV_Fraction) * 16.0f + 0.5f);
		
		USART3_BRR = (uint16_t)(USART3_DIV_Mantissa << 4 | USART3_DIV_Fraction);
	}
	if(Baudrate == 115200){
		USART3_DIV_Baudrate = (float)(F_CLK / (16.0f * Baudrate));
		USART3_DIV_Mantissa = (uint8_t)(USART3_DIV_Baudrate);
		USART3_DIV_Fraction = (uint8_t)((USART3_DIV_Baudrate - USART3_DIV_Fraction) * 16.0f + 0.5f);
		
		USART3_BRR = (uint16_t)(USART3_DIV_Mantissa << 4 | USART3_DIV_Fraction);
	}
	
	//Config USART2 
	USART3_CR1 |= (1 << 3);
	USART3_CR1 |= (1 << 13);
	USART3_CR1 |= (1 << 7);
}

void USART3_SendChar(const char c){
	while(!(USART3_SR & (1 << 7))){};
		USART3_DR = c;
}

void USART3_SendString(const char *str){
	while(*str){
		USART3_SendChar(*str++);
	}
}

