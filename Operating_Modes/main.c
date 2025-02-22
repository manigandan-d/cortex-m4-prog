#include <stdio.h>
#include <stdint.h>

#define NVIC_ISER0   ((volatile uint32_t *)0xE000E100)
#define NVIC_STIR    ((volatile uint32_t *)0xE000EF00)

#define IRQ_NUMBER   3

void generate_interrupt(void) {
    *NVIC_ISER0 |= (1 << IRQ_NUMBER);

    *NVIC_STIR = (IRQ_NUMBER & 0x1FF);
}

int main(void) {
    printf("In thread mode: Before interrupt\n");

    generate_interrupt();

    printf("In thread mode: After interrupt\n");

    while (1);
}

void RTC_WKUP_IRQHandler(void) {
    printf("In handler mode: ISR executed\n");
}
