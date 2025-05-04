#include <stdio.h>
#include <stdint.h>

int main(void) {
    uint32_t data = 10;

    // Move variable data to R0
    __asm volatile ("MOV R0, %0" :: "r"(data));

    // Move immediate value 0xC1 to R1
    __asm volatile ("MOV R1, %0" :: "i"(0xC1));

    while(1);
}
