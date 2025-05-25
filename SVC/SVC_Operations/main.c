#include <stdio.h>
#include <stdint.h>

/**
 * Add two integers using an SVC.
 */
int32_t add(int32_t x, int32_t y) {
    int32_t res = 0;

    // Trigger SVC #1 for addition
    __asm volatile ("SVC #1");
    // Move the result from R0 to the C variable 'res'
    __asm volatile ("MOV %0, R0": "=r"(res));

    return res;
}

/**
 * Subtract two integers using an SVC.
 */
int32_t sub(int32_t x, int32_t y) {
    int32_t res = 0;

    // Trigger SVC #2 for subtraction
    __asm volatile ("SVC #2");
    __asm volatile ("MOV %0, R0": "=r"(res));

    return res;
}

/**
 * Multiply two integers using an SVC.
 */
int32_t mul(int32_t x, int32_t y) {
    int32_t res = 0;

    // Trigger SVC #3 for multiplication
    __asm volatile ("SVC #3");
    __asm volatile ("MOV %0, R0": "=r"(res));

    return res;
}

/**
 * Divide two integers using an SVC.
 */
int32_t div(int32_t x, int32_t y) {
    int32_t res = 0;

    // Trigger SVC #4 for division
    __asm volatile ("SVC #4");
    __asm volatile ("MOV %0, R0": "=r"(res));

    return res;
}

int main(void) {
    int32_t a = 10, b = 5, res = 0;

    res = add(a, b);
    printf("Addition result: %ld\n", res);

    res = sub(a, b);
    printf("Subtraction result: %ld\n", res);

    res = mul(a, b);
    printf("Multiplication result: %ld\n", res);

    res = div(a, b);
    printf("Division result: %ld\n", res);

    while(1);
}

/**
 * Naked SVC Handler: Simply gets the MSP and branches to the C handler.
 */
__attribute__((naked)) void SVC_Handler(void) {
    __asm volatile ("MRS R0, MSP");  // Load MSP (Main Stack Pointer) into R0
    __asm volatile ("B handle_svc"); // Branch to the C handler
}

/**
 * C-level handler for SVC calls.
 * Extracts SVC number and performs the corresponding operation.
 */
void handle_svc(uint32_t *pBaseOfStackFrame) {
    int32_t arg0, arg1, res = 0;

    // pBaseOfStackFrame[6] contains the PC at the time of the SVC call
    uint8_t *pReturn_addr = (uint8_t *)pBaseOfStackFrame[6];
    pReturn_addr -= 2; // Point to the SVC instruction

    // Extract the SVC number
    uint8_t svc_num = *pReturn_addr;

    // Retrieve arguments from the stack frame (R0 and R1)
    arg0 = pBaseOfStackFrame[0];
    arg1 = pBaseOfStackFrame[1];

    // Perform the requested operation based on the SVC number
    switch(svc_num) {
        case 1:
            res = arg0 + arg1;
            break;
        case 2:
            res = arg0 - arg1;
            break;
        case 3:
            res = arg0 * arg1;
            break;
        case 4:
            res = arg0 / arg1;
            break;
        default:
            printf("Invalid SVC number: %d\n", svc_num);
    }

    // Return the result in R0 by updating the stack frame
    pBaseOfStackFrame[0] = res;
}
