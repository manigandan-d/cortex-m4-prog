#include <stdio.h>
#include <stdint.h>

int main(void) {
	__asm volatile ("SVC #15");

//	register uint32_t data __asm ("r0");

	uint32_t data = 0;
	__asm volatile ("MOV %0, R0": "=r"(data));

	printf("Data: %ld\n", data);

	for(;;);
}

__attribute__((naked)) void SVC_Handler(void) {
	__asm volatile ("MRS R0, MSP");

	__asm volatile ("B SVC_Handler_C");
}

void SVC_Handler_C(uint32_t *pMSP) {
	printf("SVC Handler\n");

	uint8_t *pPC = (uint8_t *)pMSP[6];
	pPC -= 2;

	uint8_t SVC_Num = *pPC;
	printf("SVC Number: %d\n", SVC_Num);

	SVC_Num += 4;

	pMSP[0] = SVC_Num;
}

