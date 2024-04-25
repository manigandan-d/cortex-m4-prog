#include <stdio.h>
#include <stdint.h>

int divide(int x, int y) {
	return x/y;
}

int main(void) {
	uint32_t *pSHCSR = (uint32_t *)0xE000ED24;

	*pSHCSR |= (1 << 16);
	*pSHCSR |= (1 << 17);
	*pSHCSR |= (1 << 18);

#if 0
	uint32_t *pSRAM = (uint32_t *)0x20010000;
	*pSRAM = 0xFFFFFFFF;

	void (*fptr)(void) = (void *)0x20010001;
//	void (*fptr)(void) = (void *)0x20010000;
	fptr();
#endif

	uint32_t *pCCR = (uint32_t *)0xE000ED14;
	*pCCR |= (1 << 4);

	int res = divide(10, 0);
	printf("Result: %d\n", res);

	for(;;);
}

void HardFault_Handler(void) {
	printf("Hard Fault\n");
	while(1);
}

void MemManage_Handler(void) {
	printf("Memory Manage\n");
	while(1);
}

void BusFault_Handler(void) {
	printf("Bus Fault\n");
	while(1);
}

__attribute__((naked)) void UsageFault_Handler(void) {
	__asm volatile ("MRS r0, MSP");

	__asm volatile ("B UsageFault_Handler_C");
}

void UsageFault_Handler_C(uint32_t *pMSP) {
	uint32_t *pUFSR = (uint32_t *)0xE000ED2A;

	printf("Usage Fault\n");

	printf("UFSR: %lx\n", *pUFSR);

	printf("MSP: %p\n", pMSP);

	printf("R0: %lx\n", pMSP[0]);
	printf("R1: %lx\n", pMSP[1]);
	printf("R2: %lx\n", pMSP[2]);
	printf("R3: %lx\n", pMSP[3]);
	printf("R12: %lx\n", pMSP[4]);
	printf("LR: %lx\n", pMSP[5]);
	printf("PC: %lx\n", pMSP[6]);
	printf("XPSR: %lx\n", pMSP[7]);

	while(1);
}

