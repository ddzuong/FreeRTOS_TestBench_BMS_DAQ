//GPIO_CONFIG
//GPIO_WRITEPIN
//GPIO_READPIN

#include <stdint.h>
#include "gpio.h"
void GPIO_Config(volatile GPIO_TypeDef *GPIOx, uint16_t PIN, uint32_t MODE){
	uint32_t POS = 0; //position
	uint32_t CONFIG = 0;
	for(POS = 0; POS < 16; POS++){
		if(PIN & (1 << POS)){
			//Clear 4 bit config
		if(POS < 8){
			//CRL
			GPIOx->CRL.reg &= ~(uint32_t)(0xF << (POS*4));
		}
		else{
		//CRH
			GPIOx->CRH.reg &= ~(uint32_t)(0xF << ((POS-8)*4));
		}
		
		switch(MODE){
			case GPIO_MODE_OUTPUT_PP:
				CONFIG = (0x03 << 0) | (0x00 << 2);
				break;
			case GPIO_MODE_OUTPUT_OD:
				CONFIG = (0x03 << 0) | (0x01 << 2);
				break;
			case GPIO_MODE_OUTPUT_AF_PP:
				CONFIG = (0x03 << 0) | (0x02 << 2);
				break;
			case GPIO_MODE_OUTPUT_AF_OD:
				CONFIG = (0x03 << 0) | (0x03 << 2);
				break;
			case GPIO_MODE_INPUT_ANALOG:
				CONFIG = (0x00 << 2);
				break;
			case GPIO_MODE_INPUT_FLOATING:
				CONFIG = (0x01 << 2);
				break;
			case GPIO_MODE_INPUT_PU:
				CONFIG = (0x02 << 2);
				GPIOx->ODR.reg |= (1 << POS);
				break;
			case GPIO_MODE_INPUT_PD:
				CONFIG = (0x02 << 2);
				GPIOx->ODR.reg &= ~(uint32_t)(1 << POS);
		}
		
		if(POS < 8){
			GPIOx->CRL.reg |= (CONFIG << (POS*4));
		}
		else{
			GPIOx->CRH.reg |= (CONFIG << ((POS-8)*4));
		}
		}
	}
}

void GPIO_Write_Pin(volatile GPIO_TypeDef *GPIOx, uint16_t PIN, uint8_t state){
	if(state){
		GPIOx->BSRR.reg = PIN;
	}
	else{
		GPIOx->BRR.reg = PIN;
	}
}

uint8_t GPIO_Read_Pin(volatile GPIO_TypeDef *GPIOx, uint16_t PIN){
	return ((GPIOx->IDR.reg & PIN)? 1 : 0);
}