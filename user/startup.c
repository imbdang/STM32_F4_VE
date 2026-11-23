/*
 * Minimal startup for STM32F407 (Cortex-M4), written in C.
 *
 * On reset the CPU hardware:
 *   1. reads vector_table[0] -> SP  (stack is ready, so C can run)
 *   2. reads vector_table[1] -> PC  (jumps to Reset_Handler)
 */
#include <stdint.h>

/* Labels defined in linker.ld (use their ADDRESS: &_sdata) */
extern uint32_t _estack;
extern uint32_t _sidata, _sdata, _edata;
extern uint32_t _sbss, _ebss;

int main(void);

void Reset_Handler(void)
{
    /* 1. Copy .data initial values: FLASH -> RAM */
    uint32_t *src = &_sidata;
    for (uint32_t *dst = &_sdata; dst < &_edata;) {
        *dst++ = *src++;
    }

    /* 2. Fill .bss with 0 */
    for (uint32_t *dst = &_sbss; dst < &_ebss;) {
        *dst++ = 0;
    }

    /* 3. Run the application */
    main();

    while (1) {
    }
}

/* Any interrupt without its own handler ends here */
void Default_Handler(void)
{
    while (1) {
    }
}

/*
 * "weak" = default version. If you write a function with the same name
 * (e.g. SysTick_Handler in main.c), yours is used instead.
 */
void NMI_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void)  __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void)        __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void)   __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void)     __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void)    __attribute__((weak, alias("Default_Handler")));

typedef void (*handler_t)(void);

/*
 * Vector table (PM0214, section 2.3.4). linker.ld puts it at the start of FLASH.
 * To add peripheral interrupts later: IRQ number n goes at index 16 + n
 * (RM0090, Table 61). Example: ADC = IRQ 18 -> index 34.
 */
__attribute__((section(".isr_vector"), used))
const handler_t vector_table[] = {
    (handler_t)&_estack,   /*  0: initial SP (not a function!) */
    Reset_Handler,         /*  1: Reset */
    NMI_Handler,           /*  2 */
    HardFault_Handler,     /*  3 */
    MemManage_Handler,     /*  4 */
    BusFault_Handler,      /*  5 */
    UsageFault_Handler,    /*  6 */
    0, 0, 0, 0,            /*  7-10: reserved */
    SVC_Handler,           /* 11 */
    DebugMon_Handler,      /* 12 */
    0,                     /* 13: reserved */
    PendSV_Handler,        /* 14 */
    SysTick_Handler,       /* 15 */
};
