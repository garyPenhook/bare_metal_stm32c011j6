#include "main.h"

// Vector table entries needed for system startup
void NMI_Handler(void) { while(1) {} }
void HardFault_Handler(void) { while(1) {} }
void SVC_Handler(void) {}
void PendSV_Handler(void) {}
void SysTick_Handler(void) {}

// Define the vector table
__attribute__((section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    (void (*)(void))((uint32_t)0x20001800),  // Initial stack pointer value (top of RAM)
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    SVC_Handler,
    0,                                      // Reserved
    0,                                      // Reserved
    PendSV_Handler,
    SysTick_Handler,
    // External interrupts would follow here
};

// Reset handler
void Reset_Handler(void)
{
    // Initialize data section
    extern uint32_t _sidata, _sdata, _edata;
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }
    
    // Initialize BSS section to zero
    extern uint32_t _sbss, _ebss;
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }
    
    // Call system initialization
    SystemInit();
    
    // Call main
    main();
    
    // Infinite loop in case main returns
    while (1) {}
}