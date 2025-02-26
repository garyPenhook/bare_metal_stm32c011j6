#include "main.h"
#include "stm32_peripherals.h"

// Only keep system-related functions
void SystemClock_Config(void)
{
    // Enable HSI clock (internal high-speed oscillator)
    // Set the HSI to run at 48MHz
    
    // Reset the RCC clock configuration
    RCC->CR |= 0x00000001U;      // Enable HSI oscillator
    
    // Wait till HSI is ready
    while((RCC->CR & 0x00000002U) == 0);
    
    // Configure the clock source and prescalers
    RCC->CFGR = 0x00000000;      // HSI as system clock, no prescalers

    // Configure SysTick to use the system clock
    SysTick->LOAD = (uint32_t)(48000 - 1UL);    // Set reload for 1ms (48MHz / 1000Hz)
    SysTick->VAL = 0UL;                          // Reset the counter
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | // Use processor clock
                   SysTick_CTRL_ENABLE_Msk;      // Enable SysTick
}

void SystemInit(void)
{
    // Enable GPIOA clock
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    
    // Enable TIM3 clock
    RCC->APBENR1 |= RCC_APBENR1_TIM3EN;
    
    // Initialize the system clock
    SystemClock_Config();
    
    // Initialize GPIO
    GPIO_Init();
    
    // Enable SysTick interrupts
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    
    // Enable global interrupts
    __enable_irq();
}

void Error_Handler(void)
{
    while(1) {
        // Infinite loop
    }
}
