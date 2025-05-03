#include <stdio.h>
#include <stdint.h>

void display(void);
void HardFault_Handler(void);

// Define function address and its variants
#define DISPLAY_FUNC_ADDR         ((uint32_t)&display)         // Actual address of display function
#define FUNC_ADDR_NO_T_BIT        ((void *)(DISPLAY_FUNC_ADDR & ~1U))  // Clears T-bit (invalid for Thumb mode)
#define FUNC_ADDR_WITH_T_BIT      ((void *)(DISPLAY_FUNC_ADDR | 1U))   // Sets T-bit (valid for Thumb mode)

// Target function to be called via pointer
void display(void) {
    printf("Hello from display function\n");
}

int main(void) {
    printf("=== T-bit Demo ===\n");
    printf("Function 'display' is located at address: 0x%08lX\n", (unsigned long)DISPLAY_FUNC_ADDR);

    printf("Before calling function pointer...\n");

    // Choose ONE of the following cases to test:

    // Case 1: Let the compiler assign (safe)
    // void (*fptr)(void) = display;

    // Case 2: Manual assignment WITH T-bit set (safe)
    void (*fptr)(void) = FUNC_ADDR_WITH_T_BIT;

    // Case 3: Manual assignment WITHOUT T-bit set (unsafe; triggers HardFault)
    // void (*fptr)(void) = FUNC_ADDR_NO_T_BIT;

    printf("Calling function at address: %p\n", fptr);
    fptr();  // May trigger HardFault if T-bit is not set

    printf("After function call\n");

    while(1);
}

// Custom HardFault handler to detect invalid calls
void HardFault_Handler(void) {
    printf("💥 Hard fault detected!\n");
    while(1);
}
