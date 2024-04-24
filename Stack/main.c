#include <stdio.h>
#include <stdint.h>

__attribute__((naked)) void change_sp_to_psp(void) {
	__asm volatile (".equ SRAM_END, (0x20000000 + (128 * 1024))");
	__asm volatile (".equ STACK_PSP_START, (SRAM_END - 512)");

	__asm volatile ("LDR R0, =STACK_PSP_START");
	__asm volatile ("MSR PSP, R0");

	__asm volatile ("MRS R1, CONTROL");
	__asm volatile ("ORR R1, R1, #0x02");
	__asm volatile ("MSR CONTROL, R1");

	__asm volatile ("BX LR");
}

void generate_interrupts(void) {
	__asm volatile ("SVC #0x05");
}

int add(int a, int b, int c, int d) {
	return a+b+c+d;
}

int main(void) {
	change_sp_to_psp();

	int res = 0;
	res = add(7, 5, 2, 1);

	printf("Result: %d\n", res);

	generate_interrupts();

	for(;;);
}

void SVC_Handler(void) {
	printf("In SVC Handler\n");
}
