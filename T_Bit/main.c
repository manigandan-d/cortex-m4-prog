#include <stdio.h>
#include <stdint.h>

void display(void) {
	printf("Hello\n");
}

int main(void) {
	printf("Before calling\n");

	void (*fptr)(void) = display;
//	void (*fptr)(void) = (void *)0x8000cf8;
//	void (*fptr)(void) = (void *)0x8000cf9;
	fptr();

	printf("After calling\n");

	while(1);
}

void HardFault_Handler(void) {
	printf("Hard fault detected\n");

	while(1);
}
