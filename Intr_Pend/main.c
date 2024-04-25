#include <stdio.h>
#include <stdint.h>

#define __vo volatile

#define ENABLE 1
#define DISABLE 0

#define NVIC_ISER0 ((__vo uint32_t *)0xE000E100)
#define NVIC_ISER1 ((__vo uint32_t *)0xE000E104)
#define NVIC_ISER2 ((__vo uint32_t *)0xE000E108)

#define NVIC_ICER0 ((__vo uint32_t *)0XE000E180)
#define NVIC_ICER1 ((__vo uint32_t *)0XE000E184)
#define NVIC_ICER2 ((__vo uint32_t *)0XE000E188)

#define IRQ_NO_USART1 37
#define IRQ_NO_USART2 38
#define IRQ_NO_USART3 39

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
	if(EnorDi == ENABLE) {
		if(IRQNumber <= 31) {
			*NVIC_ISER0 |= (1 << IRQNumber);
		}

		else if(IRQNumber > 31 && IRQNumber < 64) {
			*NVIC_ISER1 |= (1 << (IRQNumber % 32));
		}

		else if(IRQNumber >= 64 && IRQNumber < 96) {
			*NVIC_ISER2 |= (1 << (IRQNumber % 64));
		}
	}

	else {
		if(IRQNumber <= 31) {
			*NVIC_ICER0 |= (1 << IRQNumber);
		}

		else if(IRQNumber > 31 && IRQNumber < 64) {
			*NVIC_ICER1 |= (1 << (IRQNumber % 32));
		}
		else if(IRQNumber >= 64 && IRQNumber < 96) {
			*NVIC_ICER2 |= (1 << (IRQNumber % 64));
		}
	}
}

int main(void) {
	uint32_t *pISPR1 = (uint32_t *)0xE000E204;
	*pISPR1 |= (1 << (IRQ_NO_USART3 % 32));

	USART_IRQInterruptConfig(IRQ_NO_USART3, ENABLE);

//	uint32_t *pISER1 = (uint32_t *)0xE000E104;
//	*pISER1 |= (1 << (IRQ_NO_USART3 % 32));

	USART_IRQInterruptConfig(IRQ_NO_USART3, DISABLE);

	for(;;);
}

void USART1_IRQHandler(void) {
	printf("USART1 ISR\n");
}

void USART2_IRQHandler(void) {
	printf("USART2 ISR\n");
}

void USART3_IRQHandler(void) {
	printf("USART3 ISR\n");
}

