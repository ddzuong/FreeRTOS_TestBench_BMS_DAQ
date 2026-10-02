#ifndef __TYPE_H
#define __TYPE_H

#include <stdint.h>

typedef union{
	uint32_t reg;
	struct{
		uint32_t BIT0;
		uint32_t BIT1;
		uint32_t BIT2;
		uint32_t BIT3;
		uint32_t BIT4;
		uint32_t BIT5;
		uint32_t BIT6;
		uint32_t BIT7;
		uint32_t BIT8;
		uint32_t BIT9;
		uint32_t BIT10;
		uint32_t BIT11;
		uint32_t BIT12;
		uint32_t BIT13;
		uint32_t BIT14;
		uint32_t BIT15;
		uint32_t BIT16;
		uint32_t BIT17;
		uint32_t BIT18;
		uint32_t BIT19;
		uint32_t BIT20;
		uint32_t BIT21;
		uint32_t BIT22;
		uint32_t BIT23;
		uint32_t BIT24;
		uint32_t BIT25;
		uint32_t BIT26;
		uint32_t BIT27;
		uint32_t BIT28;
		uint32_t BIT29;
		uint32_t BIT30;
		uint32_t BIT31;
	}BITS;
}__32BIT;

#endif