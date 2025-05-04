#include <stdio.h>
#include <stdint.h>

// Macros for enabling/disabling interrupt
#define ENABLE  1
#define DISABLE 0

// Base address of NVIC priority registers
#define NVIC_PR_BASEADDR ((volatile uint32_t *)0xE000E400)

// Number of implemented priority bits (Cortex-M4 typically implements upper 4 bits)
#define NO_PR_BITS_IMPLEMENTED 4

// NVIC ISER (Interrupt Set-Enable Registers)
#define NVIC_ISER0 ((volatile uint32_t *)0xE000E100)
#define NVIC_ISER1 ((volatile uint32_t *)0xE000E104)
#define NVIC_ISER2 ((volatile uint32_t *)0xE000E108)

// NVIC ICER (Interrupt Clear-Enable Registers)
#define NVIC_ICER0 ((volatile uint32_t *)0XE000E180)
#define NVIC_ICER1 ((volatile uint32_t *)0XE000E184)
#define NVIC_ICER2 ((volatile uint32_t *)0XE000E188)

// IRQ numbers (from vector table offset)
#define IRQ_NO_TIM2     28
#define IRQ_NO_I2C1_EV  31

// Pointer to Interrupt Set-Pending Register 0
uint32_t *pISPR0 = (uint32_t *)0xE000E200;

// Function to enable or disable an IRQ
void IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) {
    if (EnorDi == ENABLE) {
        if (IRQNumber <= 31)
            *NVIC_ISER0 |= (1 << IRQNumber);
        else if (IRQNumber <= 63)
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        else if (IRQNumber < 96)
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
    }

    else {
        if (IRQNumber <= 31)
            *NVIC_ICER0 |= (1 << IRQNumber);
        else if (IRQNumber <= 63)
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        else if (IRQNumber < 96)
            *NVIC_ICER2 |= (1 << (IRQNumber % 64));
    }
}

// Function to set the priority of an IRQ
void IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority) {
    uint8_t iprx = IRQNumber / 4;               // Index into priority register array
    uint8_t iprx_section = IRQNumber % 4;       // Offset within the register
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    // Write priority into the relevant section of the register
    *(NVIC_PR_BASEADDR + iprx) |= (IRQPriority << shift_amount);
}

int main(void) {
    // Assign priorities to the IRQs
    IRQPriorityConfig(IRQ_NO_TIM2, 8);      // Lower priority
    IRQPriorityConfig(IRQ_NO_I2C1_EV, 6);   // Higher priority

    // Set TIM2 interrupt as pending
    *pISPR0 |= (1 << (IRQ_NO_TIM2 % 32));

    // Enable both IRQs in NVIC
    IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
    IRQInterruptConfig(IRQ_NO_TIM2, ENABLE);

    while(1);
}

// TIM2 ISR - simulates interrupt chaining by manually pends I2C1_EV from here
void TIM2_IRQHandler(void) {
    printf("TIM2 ISR\n");

    // Pend I2C1_EV interrupt manually from inside TIM2 ISR
    *pISPR0 |= (1 << (IRQ_NO_I2C1_EV % 32));

    while(1);
}

// I2C1 Event ISR
void I2C1_EV_IRQHandler(void) {
    printf("I2C1_EV ISR\n");
}
