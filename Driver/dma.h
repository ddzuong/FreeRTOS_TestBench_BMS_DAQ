#ifndef __DMA_H
#define __DMA_H
#include <stdint.h>
#include <stdbool.h>

//---------------BASE_ADDRESS------------------
#define DMA_BASE_ADD		0x40020000

//---------------REGISTER ADDRESS----------------------

#define DMA_ISR_OFFSET		(0x000)
#define DMA_IFCR_OFFSET		(0x004)
#define DMA_CCR1_OFFSET		(0x008)
#define DMA_CNDTR1_OFFSET	(0x00C)
#define DMA_CPAR1_OFFSET	(0x010)
#define DMA_CMAR1_OFFSET	(0x014)
#define DMA_CCR2_OFFSET		(0X01C)
#define DMA_CNDTR2_OFFSET	(0x020)
#define DMA_CPAR2_OFFSET	(0x024)
#define DMA_CMAR2_OFFSET	(0x028)
#define DMA_CCR3_OFFSET		(0x030)
#define DMA_CNDTR3_OFFSET	(0x034)
#define DMA_CPAR3_OFFSET	(0x038)
#define DMA_CMAR3_OFFSET	(0x03C)
#define DMA_CCR4_OFFSET		(0X044)
#define DMA_CNDTR4_OFFSET	(0x048)
#define DMA_CPAR4_OFFSET	(0x04C)
#define DMA_CMAR4_OFFSET	(0x050)
#define DMA_CCR5_OFFSET		(0x058)
#define DMA_CNDTR5_OFFSET	(0x05C)
#define DMA_CPAR5_OFFSET	(0x060)
#define DMA_CMAR5_OFFSET	(0x064)
#define DMA_CCR6_OFFSET		(0X06C)
#define DMA_CNDTR6_OFFSET	(0x070)
#define DMA_CPAR6_OFFSET	(0x074)
#define DMA_CMAR6_OFFSET	(0x078)
#define DMA_CCR7_OFFSET		(0X080)
#define DMA_CNDTR7_OFFSET	(0x084)
#define DMA_CPAR7_OFFSET	(0x088)
#define DMA_CMAR7_OFFSET	(0x08C)

//---------------------DMA-----------------------


#define DMA_ISR				*((volatile uint32_t *)(DMA_BASE_ADD + DMA_ISR_OFFSET	))
#define DMA_IFCR			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_IFCR_OFFSET	))
#define DMA_CCR1			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CCR1_OFFSET	))
#define DMA_CNDTR1		*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CNDTR1_OFFSET))
#define DMA_CPAR1			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CPAR1_OFFSET))
#define DMA_CMAR1			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CMAR1_OFFSET)	)
#define DMA_CCR				*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CCR2_OFFSET	))
#define DMA_CNDTR2		*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CNDTR2_OFFSET)	)
#define DMA_CPAR2			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CPAR2_OFFSET)	)
#define DMA_CMAR2			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CMAR2_OFFSET)	)
#define DMA_CCR3			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CCR3_OFFSET	))
#define DMA_CNDTR3		*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CNDTR3_OFFSET)	)
#define DMA_CPAR3			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CPAR3_OFFSET)	)
#define DMA_CMAR3			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CMAR3_OFFSET)	)
#define DMA_CCR4			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CCR4_OFFSET	))
#define DMA_CNDTR4		*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CNDTR4_OFFSET)	)
#define DMA_CPAR4			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CPAR4_OFFSET)	)
#define DMA_CMAR4			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CMAR4_OFFSET)	)
#define DMA_CCR5			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CCR5_OFFSET	))
#define DMA_CNDTR5		*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CNDTR5_OFFSET)	)
#define DMA_CPAR5			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CPAR5_OFFSET)	)
#define DMA_CMAR5			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CMAR5_OFFSET)	)
#define DMA_CCR6			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CCR6_OFFSET	))
#define DMA_CNDTR6		*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CNDTR6_OFFSET)	)
#define DMA_CPAR6			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CPAR6_OFFSET)	)
#define DMA_CMAR6			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CMAR6_OFFSET)	)
#define DMA_CCR7			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CCR7_OFFSET	))
#define DMA_CNDTR7		*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CNDTR7_OFFSET)	)
#define DMA_CPAR7			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CPAR7_OFFSET)	)
#define DMA_CMAR7			*((volatile uint32_t *)(DMA_BASE_ADD + DMA_CMAR7_OFFSET)	)


//-----------------------API------------------------------
void DMA_ADC1_Init(volatile uint16_t *buffer, uint16_t len, uint8_t mode);
void DMA_USART1_Init(void);
bool DMA_USART1_Start(const uint8_t *buffer, uint16_t len);
void DMA1_Channel4_IRQHandler(void);
#endif
