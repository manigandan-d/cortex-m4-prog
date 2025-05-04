#include <stdio.h>
#include <stdint.h>

// SRAM Configuration for Stack Allocation
#define SRAM_START      0x20000000UL
#define SRAM_SIZE       (128 * 1024)
#define SRAM_END        (SRAM_START + SRAM_SIZE)

// Allocate equal stack sizes for MSP and PSP
#define TOTAL_STACK_SIZE 1024U
#define MSP_SIZE         (TOTAL_STACK_SIZE / 2)
#define PSP_SIZE         (TOTAL_STACK_SIZE / 2)

#define MSP_START_ADDR   SRAM_END
#define MSP_END_ADDR     (SRAM_END - MSP_SIZE)
#define PSP_START_ADDR   MSP_END_ADDR
#define PSP_END_ADDR     (PSP_START_ADDR - PSP_SIZE)


// To determine current stack pointer mode
void check_sp_mode(void) {
    uint32_t control;

    // Read CONTROL register
    __asm volatile ("MRS %0, CONTROL" : "=r" (control));

    if (control & (1 << 1))
        printf("SP: PSP (Thread mode)\n");
    else
        printf("SP: MSP (Handler mode or default thread mode)\n");
}

// To fetch current SP value
uint32_t get_sp(void) {
    uint32_t sp;
    __asm volatile ("MOV %0, SP" : "=r" (sp));
    return sp;
}

// ------------------------------
// Naked function to switch from MSP to PSP
// ------------------------------
// Does not use function prologue/epilogue to avoid altering SP
__attribute__((naked)) void switch_to_psp(void) {
    // Load PSP address
    __asm volatile ("LDR R0, =%0" :: "i" (PSP_START_ADDR));
    __asm volatile ("MSR PSP, R0");

    // Update CONTROL register to select PSP
    __asm volatile ("MRS R1, CONTROL");
    __asm volatile ("ORR R1, R1, #2");   // Set bit 1 (SPSEL = 1)
    __asm volatile ("MSR CONTROL, R1");

    __asm volatile ("BX LR"); // Return
}

// SVC Handler: runs in Handler mode and uses MSP
void SVC_Handler(void) {
    printf("In SVC_Handler...\n");

    check_sp_mode();  // Should output MSP
    printf("SVC Handler SP = 0x%08lX\n", get_sp());
}

int main(void) {
    printf("Booting with MSP...\n");

    // At reset, MSP is active
    check_sp_mode();
    printf("SP = 0x%08lX\n", get_sp());

    // Switch to PSP in thread mode
    switch_to_psp();

    printf("\nAfter switching to PSP in thread mode:\n");
    check_sp_mode();
    printf("SP = 0x%08lX\n", get_sp());

    // Trigger SVC exception to enter Handler mode (which uses MSP)
    printf("\nTriggering SVC to enter Handler Mode...\n");
    __asm volatile ("SVC #0");

    while (1);
}
