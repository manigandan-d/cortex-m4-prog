#include <stdio.h>
#include <stdint.h>

int main() {
#if 0
	__asm volatile ("LDR R0, =#0x20001000");
	__asm volatile ("LDR R1, =#0x20001004");
	__asm volatile ("LDR R2, [R0]");
	__asm volatile ("LDR R3, [R1]");
	__asm volatile ("ADD R2, R2, R3");
	__asm volatile ("STR R2, [R0]");
#endif

	int data = 10;
	__asm volatile ("MOV R0, %0"::"r"(data));

	int controlReg = 0;
	__asm volatile ("MRS %0, CONTROL":"=r"(controlReg));
	printf("%d\n", controlReg);

	int dupData = 0;
	__asm volatile ("MOV %0, %1": "=r"(dupData): "r"(data));
	printf("%d\n", dupData);

	int val = 25;
	int *ptr = &val;
	int dupVal = 0;
	__asm volatile ("LDR %0, [%1]": "=r"(dupVal): "r"(ptr));
	printf("%d\n", dupVal);

	for(;;);
}

