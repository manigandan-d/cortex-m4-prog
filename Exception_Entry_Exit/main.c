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

void generate_interrupt() {
	uint32_t *pSTIR = (uint32_t *)0xE000EF00;
	uint32_t *pISER0 = (uint32_t *)0xE000E100;

	*pISER0 |= (1 << 3);

	*pSTIR = (3 & 0x1FF);
}

int main(void) {
//	change_sp_to_psp();

	printf("In thread mode: before interrupt\n");

	generate_interrupt();

	printf("In thread mode: after interrupt\n");

	for(;;);
}

void RTC_WKUP_IRQHandler(void) {
	printf("In handler mode: ISR\n");
}

