#include <stdio.h>
#include <stdint.h>

void generate_interrupt() {
	uint32_t *pSTIR = (uint32_t *)0xE000EF00;
	uint32_t *pISER0 = (uint32_t *)0xE000E100;

	*pISER0 |= (1 << 3);

	*pSTIR = (3 & 0x1FF);
}

int main(void) {
	printf("In thread mode: before interrupt\n");

	generate_interrupt();

	printf("In thread mode: after interrupt\n");

	for(;;);
}

void RTC_WKUP_IRQHandler(void) {
	printf("In handler mode: ISR\n");
}

