#include <stdio.h>
#include <stdint.h>

#include "led.h"

#define MAX_TASK 5

#define TASK_STACK_SIZE  1024U
#define SCHED_STACK_SIZE 1024U

#define SRAM_START 0x20000000U
#define SRAM_SIZE  ((128) * (1024))
#define SRAM_END   ((SRAM_START) + (SRAM_SIZE))

#define T1_STACK_START   SRAM_END
#define T2_STACK_START   ((SRAM_END) - (1 * TASK_STACK_SIZE))
#define T3_STACK_START 	 ((SRAM_END) - (2 * TASK_STACK_SIZE))
#define T4_STACK_START 	 ((SRAM_END) - (3 * TASK_STACK_SIZE))
#define IDLE_STACK_START ((SRAM_END) - (4 * TASK_STACK_SIZE))

#define SCHED_STACK_START ((SRAM_END) - (5 * TASK_STACK_SIZE))

#define TICK_HZ 1000U

#define HSI_CLK 		16000000U
#define SYSTICK_TIM_CLK HSI_CLK

#define DUMMY_XPSR 0x01000000U

#define READY_STATE 0x00
#define BLOCK_STATE   0xFF

#define INTR_ENABLE()  do{ __asm volatile ("MOV R0, #0x0"); __asm volatile ("MSR PRIMASK, R0"); }while(0)
#define INTR_DISABLE() do{ __asm volatile ("MOV R0, #0x1"); __asm volatile ("MSR PRIMASK, R0"); }while(0)

typedef struct {
	uint32_t psp_value;
	uint32_t block_count;
	uint8_t curr_state;
	void (*task_handler)(void);
}TCB_t;

uint8_t g_tick_count;
uint8_t curr_task = 1;

void idle_task(void);
void task1_handler(void);
void task2_handler(void);
void task3_handler(void);
void task4_handler(void);

void enable_processor_faults();
__attribute__((naked)) void init_sched_stack(uint32_t top);
void init_tasks_stack();
void init_systick_timer(uint32_t tick_hz);
uint32_t get_psp_value(void);
void set_psp_value(uint32_t val);
__attribute__((naked)) void switch_sp_to_psp(void);
void update_next_task(void);
void schedule(void);
void task_delay(uint32_t tick_count);
void unblock_tasks(void);
void update_g_tick_count(void);

TCB_t user_tasks[MAX_TASK];

int main(void) {
	enable_processor_faults();

	init_sched_stack(SCHED_STACK_START);

	init_tasks_stack();

	led_init_all();

	init_systick_timer(TICK_HZ);

	switch_sp_to_psp();

	task1_handler();

	while(1);
}

void idle_task(void) {
	while(1);
}

void task1_handler(void) {
	while(1) {
//		printf("Task_1\n");

		led_on(LED_GREEN);
		task_delay(1000);
		led_off(LED_GREEN);
		task_delay(1000);
	}
}

void task2_handler(void) {
	while(1) {
//		printf("Task_2\n");

		led_on(LED_ORANGE);
		task_delay(500);
		led_off(LED_ORANGE);
		task_delay(500);
	}
}

void task3_handler(void) {
	while(1) {
//		printf("Task_3\n");

		led_on(LED_BLUE);
		task_delay(250);
		led_off(LED_BLUE);
		task_delay(250);
	}
}

void task4_handler(void) {
	while(1) {
//		printf("Task_4\n");

		led_on(LED_RED);
		task_delay(125);
		led_off(LED_RED);
		task_delay(125);
	}
}

void enable_processor_faults() {
	uint32_t *pSHCSR = (uint32_t *)0xE000ED24;

	*pSHCSR |= (1 << 16);
	*pSHCSR |= (1 << 17);
	*pSHCSR |= (1 << 18);
}

__attribute__((naked)) void init_sched_stack(uint32_t top) {
	__asm volatile ("MSR MSP, %0": : "r"(top));
	__asm volatile ("BX LR");
}

void init_tasks_stack() {
	user_tasks[0].curr_state = READY_STATE;
	user_tasks[1].curr_state = READY_STATE;
	user_tasks[2].curr_state = READY_STATE;
	user_tasks[3].curr_state = READY_STATE;
	user_tasks[4].curr_state = READY_STATE;

	user_tasks[0].psp_value = IDLE_STACK_START;
	user_tasks[1].psp_value = T1_STACK_START;
	user_tasks[2].psp_value = T2_STACK_START;
	user_tasks[3].psp_value = T3_STACK_START;
	user_tasks[4].psp_value = T4_STACK_START;

	user_tasks[0].task_handler = idle_task;
	user_tasks[1].task_handler = task1_handler;
	user_tasks[2].task_handler = task2_handler;
	user_tasks[3].task_handler = task3_handler;
	user_tasks[4].task_handler = task4_handler;

	uint32_t *ptop;

	for(int i=0; i<MAX_TASK; i++) {
		ptop = (uint32_t *)user_tasks[i].psp_value;

		ptop--;
		*ptop = DUMMY_XPSR;

		ptop--;
		*ptop = (uint32_t)user_tasks[i].task_handler;

		ptop--;
		*ptop = 0xFFFFFFFD;

		for(int j=0; j<13; j++) {
			ptop--;
			*ptop = 0;
		}

		user_tasks[i].psp_value = (uint32_t)ptop;
	}
}

void init_systick_timer(uint32_t tick_hz) {
	uint32_t *pSCSR = (uint32_t *)0xE000E010;
	uint32_t *pSRVR = (uint32_t *)0xE000E014;

	uint32_t count = (SYSTICK_TIM_CLK / tick_hz) - 1;

	*pSRVR &= ~(0x00FFFFFF);
	*pSRVR |= count;

	*pSCSR |= (1 << 2);
	*pSCSR |= (1 << 1);

	*pSCSR |= (1 << 0);
}

uint32_t get_psp_value(void) {
	return user_tasks[curr_task].psp_value;
}

void set_psp_value(uint32_t val) {
	user_tasks[curr_task].psp_value = val;
}

__attribute__((naked)) void switch_sp_to_psp(void) {
	__asm volatile ("PUSH {LR}");
	__asm volatile ("BL get_psp_value");
	__asm volatile ("MSR PSP, R0");
	__asm volatile ("POP {LR}");

	__asm volatile ("MOV R0, #0x02");
	__asm volatile ("MSR CONTROL, R0");

	__asm volatile ("BX LR");
}

void update_next_task(void) {
	int state = BLOCK_STATE;

	for(int i=0; i<MAX_TASK; i++) {
		curr_task++;
		curr_task %= MAX_TASK;

		state = user_tasks[curr_task].curr_state;

		if((state == READY_STATE) && (curr_task !=0))
			break;
	}

	if(state != READY_STATE)
		curr_task = 0;
}

void schedule(void) {
	uint32_t *pICSR = (uint32_t *)0xE000ED04;
	*pICSR |= (1 << 28);
}

void task_delay(uint32_t tick_count) {
	INTR_DISABLE();

	if(curr_task) {
		user_tasks[curr_task].block_count = g_tick_count + tick_count;
		user_tasks[curr_task].curr_state = BLOCK_STATE;

		schedule();
	}

	INTR_ENABLE();
}

void unblock_tasks(void) {
	for(int i=1; i<MAX_TASK; i++) {
		if(user_tasks[i].curr_state == BLOCK_STATE) {
			if(user_tasks[i].block_count == g_tick_count) {
				user_tasks[i].curr_state = READY_STATE;
			}
		}
	}
}

void update_g_tick_count(void) {
	g_tick_count++;
}

void SysTick_Handler(void) {
	uint32_t *pICSR = (uint32_t *)0xE000ED04;

	update_g_tick_count();
	unblock_tasks();

	*pICSR |= (1 << 28);
}

__attribute__((naked)) void PendSV_Handler(void) {
	__asm volatile ("MRS R0, PSP");
	__asm volatile ("STMDB R0!, {R4-R11}");
	__asm volatile ("PUSH {LR}");
	__asm volatile ("BL set_psp_value");

	__asm volatile ("BL update_next_task");
	__asm volatile ("BL get_psp_value");
	__asm volatile ("LDMIA R0!, {R4-R11}");
	__asm volatile ("MSR PSP, R0");
	__asm volatile ("POP {LR}");

	__asm volatile ("BX LR");
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

void UsageFault_Handler(void) {
	printf("Usage Fault\n");
	while(1);
}
