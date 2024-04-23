#include <stdio.h>
#include <stdint.h>

#define ALIAS_BASE 0x22000000U
#define BITBAND_BASE 0x20000000U

int main(void) {
	uint8_t *ptr = (uint8_t *)0x20000200;
	*ptr = 0xFF;

//	Normal Method
	*ptr &= ~(1 << 7);

	*ptr = 0xFF;

//	Bit Band Method
	uint8_t *alias_addr = (uint8_t *)(ALIAS_BASE + (32 * (0x20000200 - BITBAND_BASE)) + 7 * 4);
	*alias_addr = 0;

	for(;;);
}
