#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t controlReg = 0;

    // Move the content of the CONTROL register to the C variable `controlReg`
    // MRS: Move Register to Special register (from system register to general-purpose)
    __asm volatile ("MRS %0, CONTROL" : "=r"(controlReg));

    // Print the value of the CONTROL register
    printf("CONTROL register value: %ld\n", controlReg);

    while(1);
}
