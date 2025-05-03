#include <stdio.h>
#include <stdint.h>

// Define AIRCR (Application Interrupt and Reset Control Register) address
#define SCB_AIRCR (*(volatile uint32_t *)0xE000ED0C)

int main(void) {
    // Read the AIRCR register
    uint32_t aircr = SCB_AIRCR;

    // Bit 15 of AIRCR indicates endianness:
    // 0 = Little-endian, 1 = Big-endian
    uint32_t endianness = (aircr >> 15) & 0x1;

    if (endianness == 0) {
        printf("Processor is Little-endian\n");
    } 
    
    else {
        printf("Processor is Big-endian\n");
    }

    while(1); 
}
