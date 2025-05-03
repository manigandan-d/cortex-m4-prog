#include <stdio.h>
#include <stdint.h>

// Generate software interrupt for IRQ3
void generate_interrupt(void) {
    // STIR (Software Trigger Interrupt Register) - Address: 0xE000EF00
    uint32_t *pSTIR  = (uint32_t*)0xE000EF00;

    // ISER0 (Interrupt Set-Enable Register 0) - Address: 0xE000E100
    uint32_t *pISER0 = (uint32_t*)0xE000E100;

    // Enable IRQ3 (interrupt line 3)
    *pISER0 |= (1 << 3);

    // Trigger IRQ3 via software (only lower 9 bits valid)
    *pSTIR = (3 & 0x1FF);
}

// Check the current processor mode by reading IPSR
void check_processor_mode(void) {
    uint32_t ipsr_value;

    // Read Interrupt Program Status Register (IPSR)
    __asm volatile ("MRS %0, IPSR" : "=r" (ipsr_value));

    if (ipsr_value == 0) {
        // Thread mode (normal execution)
        printf("Processor is in Thread Mode.\n");
    }

    else {
        // Handler mode (executing an ISR)
        printf("Processor is in Handler Mode. ISR_NUMBER: %ld\n", ipsr_value);

        if (ipsr_value == 15) {
            printf("Currently in SysTick Handler.\n");
        }

        else if (ipsr_value == 16) {
            printf("Currently in IRQ0 Handler.\n");
        }

        else {
            // Adjusting offset (IRQ number = IPSR - 16)
            printf("Currently in IRQ%ld Handler.\n", ipsr_value - 16);
        }
    }
}

int main(void) {
    printf("In thread mode: Checking processor mode...\n");
    check_processor_mode();

    // Generate interrupt to transition to Handler Mode
    generate_interrupt();

    while(1);
}

// IRQ3 Handler (RTC Wakeup interrupt used for demonstration)
void RTC_WKUP_IRQHandler(void) {
    printf("In handler mode: Checking processor mode...\n");
    check_processor_mode();
}
