#include <stdio.h>
#include <stdint.h>

int32_t add(int32_t x, int32_t y) {
	int32_t res = 0;

	__asm volatile ("SVC #1");
	__asm volatile ("MOV %0, R0": "=r"(res));

	return res;
}

int32_t sub(int32_t x, int32_t y) {
	int32_t res = 0;

	__asm volatile ("SVC #2");
	__asm volatile ("MOV %0, R0": "=r"(res));

	return res;
}

int32_t mul(int32_t x, int32_t y) {
	int32_t res = 0;

	__asm volatile ("SVC #3");
	__asm volatile ("MOV %0, R0": "=r"(res));

	return res;
}

int32_t div(int32_t x, int32_t y) {
	int32_t res = 0;

	__asm volatile ("SVC #4");
	__asm volatile ("MOV %0, R0": "=r"(res));

	return res;
}

int main(void) {
	int32_t a = 12, b = 5, res = 0;

	res = add(a, b);
	printf("ADD: %ld\n", res);

	res = sub(a, b);
	printf("SUB: %ld\n", res);

	res = mul(a, b);
	printf("MUL: %ld\n", res);

	res = div(a, b);
	printf("DIV: %ld\n", res);

	for(;;);
}

__attribute__((naked)) void SVC_Handler(void) {
	__asm volatile ("MRS R0, MSP");

	__asm volatile ("B SVC_Handler_C");
}

void SVC_Handler_C(uint32_t *pMSP) {
	int32_t arg0, arg1, res;
	arg0 = arg1 = res = 0;

	uint8_t *pPC = (uint8_t *)pMSP[6];
	pPC -= 2;

	uint8_t SVC_Num = *pPC;

	arg0 = pMSP[0];
	arg1 = pMSP[1];

	switch(SVC_Num) {
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
		printf("Invalid SVC Code\n");
	}

	pMSP[0] = res;
}

