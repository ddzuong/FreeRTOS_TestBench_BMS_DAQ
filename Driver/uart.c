#include "uart.h"
#include "gpio.h"
#include <stdint.h>
#include "exti.h"
#include "dma.h"
//SR,CR, NVIC_USART, DR, BBR

//Receive data
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

void USART1_Init(uint32_t Baudrate){
	//PA9: OUTPUT AF PP
	GPIO_Config(GPIOA, GPIO_PIN_9, GPIO_MODE_OUTPUT_AF_PP);
	//Baudrate 9600b/s //8MHZ
	//PA10: INPUT FLOATING
	GPIO_Config(GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT_FLOATING);
	//USARTx_BRR = f_clk/(16*baudrate)
	if(Baudrate == 9600){
		USART1_BRR = 0x341;	//Baudrate: 9600
	}
	else if(Baudrate == 115200){
		USART1_BRR = 0x45;	//Baudrate: 115200
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

