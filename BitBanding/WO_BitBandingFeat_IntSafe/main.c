#include <stdint.h>
#include <stdio.h>

// Mock functions to enable/disable interrupts
void disable_interrupts(void) {
    __asm volatile("CPSID i"); // Disable interrupts
}

void enable_interrupts(void) {
    __asm volatile("CPSIE i"); // Enable interrupts
}

void interrupt_safe_bit_manipulation(volatile uint32_t *addr, uint8_t bit_number) {
    uint32_t value;

    // Disable interrupts before accessing shared resource
    disable_interrupts();

    // Read current value
    value = *addr;

    // Set bit
    value |= (1 << bit_number);
    *addr = value;

    // Clear bit
    value &= ~(1 << bit_number);
    *addr = value;

    // Toggle bit
    value ^= (1 << bit_number);
    *addr = value;

    // Enable interrupts after manipulation is complete
    enable_interrupts();
}

int main() {
    uint32_t data = 0x00;  // Initialize some memory location
    uint32_t *ptr = &data;

    interrupt_safe_bit_manipulation(ptr, 5);  // Operate on bit 5
    printf("Final Value: 0x%08lX\n", data);

    while(1);
}
