#ifndef __AFIO_H
#define __AFIO_H

#include <stdint.h>

typedef struct{
	volatile union{
		uint32_t reg;
		struct{
			uint32_t PIN			:4;
			uint32_t PORT			:3;
			uint32_t EVOE			:1;
			uint32_t Res1			:24;
		}BITS;
	}EVCR;
	
	uint32_t MAPR;
	
	volatile union{
		uint32_t reg;
		struct{
			uint32_t EXTI0		:4;
			uint32_t EXTI1		:4;
			uint32_t EXTI2		:4;
			uint32_t EXTI3		:4;
			uint32_t Res1			:16;
		}BITS;
	}EXTICR1;
	
	volatile union{
		uint32_t reg;
		struct{
			uint32_t EXTI4		:4;
			uint32_t EXTI5		:4;
			uint32_t EXTI6		:4;
			uint32_t EXTI7		:4;
			uint32_t Res1			:16;
		}BITS;
	}EXTICR2;
	
	volatile union{
		uint32_t reg;
		struct{
			uint32_t EXTI8		:4;
			uint32_t EXTI9		:4;
			uint32_t EXTI10		:4;
			uint32_t EXTI11		:4;
			uint32_t Res1			:16;
		}BITS;
	}EXTICR3;
	
	volatile union{
		uint32_t reg;
		struct{
			uint32_t EXTI12		:4;
			uint32_t EXTI13		:4;
			uint32_t EXTI14		:4;
			uint32_t EXTI15		:4;
			uint32_t Res1			:16;
		}BITS;
	}EXTICR4;
	
	uint32_t MAPR2;
	
}AFIO_TypeDef;

#define AFIO				((volatile AFIO_TypeDef *)0x40010000UL)


#endif