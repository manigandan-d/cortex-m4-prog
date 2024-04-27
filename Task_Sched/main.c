#include <stdio.h>
#include <stdint.h>

#define SIZE_TASK_STACK 1024U
#define SIZE_SCHED_STACK 1024U

#define SRAM_START 0x20000000U
#define SIZE_SRAM ((128) * (1024))
#define SRAM_END ((SRAM_START) + (SIZE_SRAM))

#define T1_STACK_START SRAM_END
#define T2_STACK_START ((T1_STACK_START) - (1 * SIZE_TASK_STACK))
#define T3_STACK_START ((T1_STACK_START) - (2 * SIZE_TASK_STACK))
#define T4_STACK_START ((T1_STACK_START) - (3 * SIZE_TASK_STACK))

#define SCHED_STACK_START ((T1_STACK_START) - (4 * SIZE_SCHED_STACK))

#define TICK_HZ 1000U

#define HSI_CLK 16000000U
#define SYSTICK_TIM_CLK HSI_CLK

void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void SysTick_Init(uint32_t tick_hz);

int main(void) {
	SysTick_Init(TICK_HZ);

	for(;;);
}

void task1_handler(void) {
	while(1) {
		printf("Task_1\n");
	}
}

void task2_handler(void) {
	while(1) {
		printf("Task_2\n");
	}
}

void task3_handler(void) {
	while(1) {
		printf("Task_3\n");
	}
}

void task4_handler(void) {
	while(1) {
		printf("Task_4\n");
	}
}

void SysTick_Init(uint32_t tick_hz) {
	uint32_t *pSCSR = (uint32_t *)0xE000E010;
	uint32_t *pSRVR = (uint32_t *)0xE000E014;

	uint32_t count = (SYSTICK_TIM_CLK / tick_hz) - 1;

	*pSRVR &= ~(0x00FFFFFF);
	*pSRVR |= count;

	*pSCSR |= (1 << 2);
	*pSCSR |= (1 << 1);

	*pSCSR |= (1 << 0);
}

void SysTick_Handler(void) {

}
