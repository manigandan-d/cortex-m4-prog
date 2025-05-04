#include <stdio.h>
#include <stdint.h>

int main(void) {
	// Memory addresses
	uint32_t val1 = 10; // Example value at address 0x20001000
	uint32_t val2 = 20; // Example value at address 0x20001004

	// Assume val1 and val2 are located at specific memory addresses
	uint32_t *address1 = (uint32_t *)0x20001000;
	uint32_t *address2 = (uint32_t *)0x20001004;

	*address1 = val1;
	*address2 = val2;

	// Inline assembly to perform the operation
	__asm volatile ("LDR R0, =#0x20001000"); // Load the address of val1 into R0
	__asm volatile ("LDR R1, =#0x20001004"); // Load the address of val2 into R1
	__asm volatile ("LDR R2, [R0]");         // Load the value at address1 into R2
	__asm volatile ("LDR R3, [R1]");         // Load the value at address2 into R3
	__asm volatile ("ADD R2, R2, R3");       // Add the values in R2 and R3
	__asm volatile ("STR R2, [R0]");         // Store the result back into address1

	printf("Result: %ld\n", *address1);

	while(1);
}
