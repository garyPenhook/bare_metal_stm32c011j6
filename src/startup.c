#include <stdint.h>
#include "main.h"

// Forward declaration of the Reset_Handler
void Reset_Handler(void);

// Default handler for other exceptions
void Default_Handler(void) {
    while(1);
}

// Vector table - placed at the beginning of flash by the linker script
__attribute__ ((section(".vector_table")))
void (* const vector_table[])(void) = {
    // Stack pointer initialization (address of _estack)
    (void (*)(void))0x20002000,  // Replace with the actual value from your linker script
    Reset_Handler,
    Default_Handler,  // NMI_Handler
    Default_Handler,  // HardFault_Handler
    Default_Handler,  // MemManage_Handler
    Default_Handler,  // BusFault_Handler
    Default_Handler,  // UsageFault_Handler
    0, 0, 0, 0,       // Reserved entries
    Default_Handler,  // SVC_Handler
    Default_Handler,  // DebugMon_Handler
    0,                // Reserved
    Default_Handler,  // PendSV_Handler
    SysTick_Handler,  // SysTick_Handler
    
    // Device specific interrupts 
    Default_Handler,  // WWDG_IRQHandler
    Default_Handler,  // PVD_IRQHandler
    Default_Handler,  // RTC_IRQHandler
    Default_Handler,  // FLASH_IRQHandler
    Default_Handler,  // RCC_IRQHandler
    Default_Handler,  // EXTI0_1_IRQHandler
    Default_Handler,  // EXTI2_3_IRQHandler
    Default_Handler,  // EXTI4_15_IRQHandler
    Default_Handler,  // DMA1_Channel1_IRQHandler
    Default_Handler,  // DMA1_Channel2_3_IRQHandler
    Default_Handler,  // DMA1_Channel4_5_IRQHandler
    Default_Handler,  // ADC_IRQHandler
    Default_Handler,  // TIM1_BRK_UP_TRG_COM_IRQHandler
    Default_Handler,  // TIM1_CC_IRQHandler
    Default_Handler,  // TIM2_IRQHandler
    TIM3_IRQHandler,  // TIM3_IRQHandler
    Default_Handler,  // TIM14_IRQHandler
    Default_Handler,  // TIM16_IRQHandler
    Default_Handler,  // TIM17_IRQHandler
    Default_Handler,  // I2C1_IRQHandler
    Default_Handler,  // SPI1_IRQHandler
    Default_Handler,  // USART1_IRQHandler
    Default_Handler,  // USART2_IRQHandler
};

// Reset handler
void Reset_Handler(void) {
    // Copy initialized data from flash to SRAM
    extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    
    while (dst < &_edata) {
        *dst++ = *src++;
    }
    
    // Zero out the BSS section
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }
    
    // Call the system initialization
    SystemInit();
    
    // Call the main function
    main();
    
    // In case main returns, loop forever
    while(1);
}