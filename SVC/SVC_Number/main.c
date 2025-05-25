#include <stdio.h>
#include <stdint.h>

int main(void)
{
    // Trigger the SVC interrupt with SVC number 15
    __asm volatile ("SVC #15");

    uint32_t data;

    // After SVC handler finishes, move R0 (return value) into 'data' variable
    __asm volatile ("MOV %0, R0" : "=r"(data));

    printf("Data received from SVC handler: %ld\n", data);

    while(1);
}

// SVC Handler (naked to avoid compiler-generated prologue/epilogue)
__attribute__ ((naked)) void SVC_Handler(void)
{
    __asm("MRS R0, MSP");  // Load the Main Stack Pointer (MSP) into R0
    __asm("B handle_svc"); // Branch to the C function for further processing
}

// C function to handle the SVC interrupt logic
void handle_svc(uint32_t *pBaseOfStackFrame)
{
    printf("Inside SVC Handler\n");

    /*
      Stack frame:
      pBaseOfStackFrame[0] - R0
      pBaseOfStackFrame[1] - R1
      pBaseOfStackFrame[2] - R2
      pBaseOfStackFrame[3] - R3
      pBaseOfStackFrame[4] - R12
      pBaseOfStackFrame[5] - LR
      pBaseOfStackFrame[6] - PC (return address)
      pBaseOfStackFrame[7] - xPSR
    */

    // Fetch the PC to determine where SVC was called
    uint8_t *pReturn_addr = (uint8_t*)pBaseOfStackFrame[6];

    // Go back two bytes to get the SVC opcode
    pReturn_addr -= 2;

    // The byte at this location is the SVC number
    uint8_t svc_number = *pReturn_addr;
    printf("SVC Number: %d\n", svc_number);

    // Modify return value by adding 5 to the SVC number
    svc_number += 5;

    // Return the new value in R0 by updating the stack frame
    pBaseOfStackFrame[0] = svc_number;
}
