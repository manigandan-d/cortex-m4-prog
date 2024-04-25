#include <stdio.h>
#include <stdint.h>

#define __vo volatile

#define ENABLE 1
#define DISABLE 0

#define NVIC_PR_BASEADDR ((__vo uint32_t *)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED 4

#define NVIC_ISER0 ((__vo uint32_t *)0xE000E100)
#define NVIC_ISER1 ((__vo uint32_t *)0xE000E104)
#define NVIC_ISER2 ((__vo uint32_t *)0xE000E108)

#define NVIC_ICER0 ((__vo uint32_t *)0XE000E180)
#define NVIC_ICER1 ((__vo uint32_t *)0XE000E184)
#define NVIC_ICER2 ((__vo uint32_t *)0XE000E188)

#define IRQ_NO_TIM2 28
#define IRQ_NO_I2C1_EV 31

uint32_t *pISPR0 = (uint32_t *)0xE000E200;

void IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
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

void IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
	uint8_t iprx = IRQNumber / 4;
	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASEADDR + iprx) |= (IRQPriority << shift_amount);
}

int main(void) {
	IRQPriorityConfig(IRQ_NO_TIM2, 8);
	IRQPriorityConfig(IRQ_NO_I2C1_EV, 6);

	*pISPR0 |= (1 << (IRQ_NO_TIM2 % 32));

	IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	IRQInterruptConfig(IRQ_NO_TIM2, ENABLE);

	for(;;);
}

void TIM2_IRQHandler(void) {
	printf("TIM2 ISR\n");
	*pISPR0 |= (1 << (IRQ_NO_I2C1_EV % 32));
	while(1);
}

void I2C1_EV_IRQHandler(void) {
	printf("I2C1_EV ISR\n");
}

