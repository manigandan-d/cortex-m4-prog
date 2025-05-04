#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t data = 10;       // Source variable
    uint32_t dupData = 0;     // Destination variable

    // Copy content of 'data' to 'dupData' using inline assembly
    // MOV: Move value from one register (mapped from C variable) to another
    __asm volatile ("MOV %0, %1" : "=r"(dupData) : "r"(data));

    // Print the copied value
    printf("dupData: %ld\n", dupData);

    while(1);
}
