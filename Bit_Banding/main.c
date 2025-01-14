#include <stdio.h>
#include <stdint.h>

#define BITBAND_SRAM_BASE   0x22000000
#define SRAM_BASE           0x20000000

#define BITBAND_ALIAS(addr, bit)   (BITBAND_SRAM_BASE + ((addr - SRAM_BASE) * 32) + (bit * 4))

int main(void) {
	// Accessing bit 5 of 0x20000004
	volatile uint32_t *bit_addr = (uint32_t *)BITBAND_ALIAS(0x20000004, 5);

	// Set bit 5 
	*bit_addr = 1; 

	// Clear bit 5
	*bit_addr = 0;

	for(;;);
}
