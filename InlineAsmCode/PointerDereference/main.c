#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t val = 25;        // Variable holding a value
    uint32_t *ptr = &val;     // Pointer to the variable
    uint32_t dupVal = 0;      // Destination variable

    // Load the value pointed by 'ptr' into 'dupVal' using inline assembly
    __asm volatile ("LDR %0, [%1]" : "=r"(dupVal) : "r"(ptr));

    // Print the copied value
    printf("dupVal: %ld\n", dupVal);

    while(1);
}
