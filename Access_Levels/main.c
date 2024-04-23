#include <stdio.h>
#include <stdint.h>

void change_access_level_unpriv() {
	__asm volatile ("MRS R0, CONTROL");
	__asm volatile ("ORR R0, R0, #0x01");
	__asm volatile ("MSR CONTROL, R0");
}

void generate_interrupt() {
	uint32_t *pSTIR = (uint32_t *)0xE000EF00;
	uint32_t *pISER0 = (uint32_t *)0xE000E100;

	*pISER0 |= (1 << 3);

	*pSTIR = (3 & 0x1FF);
}

int main(void) {
	printf("In thread mode: before interrupt\n");

	change_access_level_unpriv();

	generate_interrupt();

	printf("In thread mode: after interrupt\n");

	for(;;);
}

void RTC_WKUP_IRQHandler(void) {
	printf("In handler mode: ISR\n");
}

void HardFault_Handler(void) {
	printf("Hard fault detected\n");
	while(1);
}
