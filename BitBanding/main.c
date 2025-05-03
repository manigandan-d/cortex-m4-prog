#include <stdint.h>
#include <stdio.h>

// Bit-band alias base for SRAM region
#define BITBAND_SRAM_BASE  0x22000000
#define SRAM_BASE          0x20000000

void bitband_example(void) {
    // Base address in SRAM where a byte will be manipulated
    volatile uint32_t *addr = (uint32_t *)0x20000200;

    // Calculate alias address to control bit 7 of the byte at 0x20000200
    // Bit-band formula: alias_addr = bitband_base + (byte_offset * 32) + (bit_number * 4)
    volatile uint32_t *alias_addr = (uint32_t *)(BITBAND_SRAM_BASE + (32 * ((uint32_t)addr - SRAM_BASE)) + (7 * 4));

    // Write 0xFF to the memory location (all bits set)
    *addr = 0xFF;

    // Clear the 7th bit using the bit-band alias address
    *alias_addr = 0;

    // Display the results
    printf("Value at original address: 0x%08lX\n", *addr);
    printf("Alias address: 0x%08lX\n", (uint32_t)alias_addr);
    printf("Value at alias address: %ld\n", *alias_addr); // Will always read back as 0 or 1
}

int main(void) {
    bitband_example();

    while(1); 
}
