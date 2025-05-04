#include <stdio.h>
#include <stdint.h>

// Macros for enabling/disabling interrupts
#define ENABLE 1
#define DISABLE 0

// NVIC register addresses for interrupt control (specific to ARM Cortex-M4)
#define NVIC_ISER1 ((volatile uint32_t *)0xE000E104)  // Interrupt Set-Enable Register 1
#define NVIC_ICER1 ((volatile uint32_t *)0xE000E184)  // Interrupt Clear-Enable Register 1
#define NVIC_ISPR1 ((volatile uint32_t *)0xE000E204)  // Interrupt Set-Pending Register 1

// IRQ numbers for USART peripherals (from vector table offsets)
#define IRQ_NO_USART1 37
#define IRQ_NO_USART2 38
#define IRQ_NO_USART3 39

// Function to enable or disable NVIC interrupt
void USART_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        // Enable interrupt for the given IRQ number using ISER1
        *NVIC_ISER1 |= (1 << (IRQNumber % 32));
    } 
    
    else {
        // Disable interrupt for the given IRQ number using ICER1
        *NVIC_ICER1 |= (1 << (IRQNumber % 32));
    }
}

int main(void) {
    // Manually set the interrupt pending flag for USART1 using ISPR1
    *NVIC_ISPR1 |= (1 << (IRQ_NO_USART1 % 32));

    // Enable USART1 interrupt in NVIC
    USART_IRQInterruptConfig(IRQ_NO_USART1, ENABLE);

    // Uncomment the following lines to test USART2 and USART3 as well:
    // *NVIC_ISPR1 |= (1 << (IRQ_NO_USART2 % 32));
    // USART_IRQInterruptConfig(IRQ_NO_USART2, ENABLE);

    // *NVIC_ISPR1 |= (1 << (IRQ_NO_USART3 % 32));
    // USART_IRQInterruptConfig(IRQ_NO_USART3, ENABLE);

    while (1);
}

// Dummy ISR definitions for demonstration (these would normally be weak symbols overridden)
void USART1_IRQHandler(void) {
    printf("USART1 ISR executed\n");
}

void USART2_IRQHandler(void) {
    printf("USART2 ISR executed\n");
}

void USART3_IRQHandler(void) {
    printf("USART3 ISR executed\n");
}
