#include <stdint.h>
#include <stdio.h>

void manipulate_bit(volatile uint32_t *addr, uint8_t bit_number) {
    // Read the current value
    uint32_t value = *addr;

    // Set bit 5
    value |= (1 << bit_number);
    *addr = value;  // Write back
    printf("Set bit %d: 0x%08X\n", bit_number, *addr);

    // Clear bit 5
    value &= ~(1 << bit_number);
    *addr = value;  // Write back
    printf("Cleared bit %d: 0x%08X\n", bit_number, *addr);

    // Toggle bit 5
    value ^= (1 << bit_number);
    *addr = value;  // Write back
    printf("Toggled bit %d: 0x%08X\n", bit_number, *addr);

    // Read bit 5
    uint8_t bit_value = (*addr & (1 << bit_number)) >> bit_number;
    printf("Read bit %d: %u\n", bit_number, bit_value);
}

int main() {
    uint32_t data = 0x00;  // Initialize some memory location
    uint32_t *ptr = &data;

    manipulate_bit(ptr, 5);  // Operate on bit 5

    while(1);
}
