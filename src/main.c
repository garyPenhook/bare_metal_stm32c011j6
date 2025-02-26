


#include <stdint.h>
#include "stm32c011xx.h"
#include "main.h"
#include "stm32c011xx.h"
#include "main.h"

// System Clock Configuration - Set up HSI at 48MHz
void SystemClock_Config(void)
{
    // Enable HSI clock (internal high-speed oscillator)
    // Set the HSI to run at 48MHz
    // For STM32C011J6M6, we'll use the HSI directly
    
    // Reset the RCC clock configuration
    RCC->CR |= 0x00000001U;      // Enable HSI oscillator
    
    // Wait till HSI is ready
    while((RCC->CR & 0x00000002U) == 0);
    
    // Configure the clock source and prescalers
    RCC->CFGR = 0x00000000;      // HSI as system clock, no prescalers
}

// Initialize GPIO pins - Configure pin A5 (LED) as output
void GPIO_Init(void)
{
    // Enable GPIOA clock
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    
    // Configure PA5 as output (assuming it connects to an LED, like on some STM boards)
    // Clear the mode bits first (MODE5[1:0])
    GPIOA->MODER &= ~GPIO_MODER_MODE5;
    // Set PA5 as output (MODE5[1:0] = 01)
    GPIOA->MODER |= GPIO_MODER_MODE5_0;
}

// Simple delay function
void Delay(uint32_t count)
{
    volatile uint32_t i;
    for(i = 0; i < count; i++);
}

// System Initialization
void SystemInit(void)
{
    // Initialize the system clock
    SystemClock_Config();
    
    // Initialize GPIO
    GPIO_Init();
}

// Error handler
void Error_Handler(void)
{
    while(1) {
        // Infinite loop
    }
}

// Main function - Simple LED blink
int main(void)
{
    // System is already initialized from startup.c before main
    
    // Main loop - toggle LED (PA5) every delay interval
    while (1) {
        // Turn on LED (PA5)
        GPIOA->BSRR = GPIO_BSRR_BS5;
        
        // Delay
        Delay(500000);
        
        // Turn off LED (PA5)
        GPIOA->BSRR = GPIO_BSRR_BR5;
        
        // Delay
        Delay(500000);
    }
    
    // Never reached
    return 0;
}