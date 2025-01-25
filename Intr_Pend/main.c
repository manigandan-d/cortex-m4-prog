#include <stdio.h>
#include <stdint.h>


#define __vo volatile
#define ENABLE 1
#define DISABLE 0

#define NVIC_ISER1 ((__vo uint32_t *)0xE000E104)
#define NVIC_ICER1 ((__vo uint32_t *)0xE000E184)
#define NVIC_ISPR1 ((__vo uint32_t *)0xE000E204)

#define IRQ_NO_USART1 37
#define IRQ_NO_USART2 38
#define IRQ_NO_USART3 39

void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        *NVIC_ISER1 |= (1 << (IRQNumber % 32));
    } else {
        *NVIC_ICER1 |= (1 << (IRQNumber % 32));
    }
}

int main(void) {
    // Manually pend USART1 interrupt
    *NVIC_ISPR1 |= (1 << (IRQ_NO_USART1 % 32));
    USART_IRQInterruptConfig(IRQ_NO_USART1, ENABLE);
    USART_IRQInterruptConfig(IRQ_NO_USART1, DISABLE);

    // For USART2 testing
    // *NVIC_ISPR1 |= (1 << (IRQ_NO_USART2 % 32));
    // USART_IRQInterruptConfig(IRQ_NO_USART2, ENABLE);
    // USART_IRQInterruptConfig(IRQ_NO_USART2, DISABLE);

    // For USART3 testing
    // *NVIC_ISPR1 |= (1 << (IRQ_NO_USART3 % 32));
    // USART_IRQInterruptConfig(IRQ_NO_USART3, ENABLE);
    // USART_IRQInterruptConfig(IRQ_NO_USART3, DISABLE);

    while (1); // Infinite loop
}

// USART ISRs
void USART1_IRQHandler(void) {
    printf("USART1 ISR executed\n");
}

void USART2_IRQHandler(void) {
    printf("USART2 ISR executed\n");
}

void USART3_IRQHandler(void) {
    printf("USART3 ISR executed\n");
}
