#include <stdio.h>

// Declare the SVC handler (executed in privileged mode)
void SVC_Handler(void);

int main(void) {
    printf("Running in Privileged Access Level (PAL).\n");

    printf("About to switch to Unprivileged Access Level (UPAL)...\n");

    // Switch to Unprivileged mode by setting CONTROL[0] = 1
    __asm volatile (
        "MRS R0, CONTROL\n\t"     // Read CONTROL register into R0
        "ORR R0, R0, #1\n\t"      // Set bit 0 to 1 (unprivileged mode)
        "MSR CONTROL, R0\n\t"     // Write back to CONTROL
    );

    // Use Supervisor Call (SVC) to invoke a handler running in privileged mode
    __asm volatile ("SVC #0");

    printf("Back to Privileged Access Level (PAL).\n");

    while (1);
}

// SVC Handler runs in privileged mode
void SVC_Handler(void) {
    printf("Privileged access granted via SVC.\n");

    // Clear CONTROL[0] to return to privileged mode
    __asm volatile (
        "MRS R0, CONTROL\n\t"     // Read CONTROL register
        "BIC R0, R0, #1\n\t"      // Clear bit 0 (back to privileged mode)
        "MSR CONTROL, R0\n\t"     // Write back to CONTROL
    );
}
