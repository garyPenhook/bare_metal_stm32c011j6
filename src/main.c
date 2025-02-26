#include <stdint.h>
#include "stm32c011xx.h"
#include "main.h"

// Define ARM Cortex-M intrinsics that might be missing
#ifndef __WFI
#define __WFI() __asm volatile ("wfi")
#endif

#ifndef __enable_irq
#define __enable_irq() __asm volatile ("cpsie i")
#endif

// Core peripherals definitions
#ifndef SysTick_BASE
#define SysTick_BASE            (0xE000E010UL)                  // SysTick Base Address
#endif

#ifndef SysTick
#define SysTick                 ((SysTick_Type *) SysTick_BASE)  // SysTick structure pointer
#endif

#ifndef SysTick_Type
typedef struct {
  volatile uint32_t CTRL;         // Offset: 0x000 (R/W)  SysTick Control and Status Register
  volatile uint32_t LOAD;         // Offset: 0x004 (R/W)  SysTick Reload Value Register
  volatile uint32_t VAL;          // Offset: 0x008 (R/W)  SysTick Current Value Register
  volatile uint32_t CALIB;        // Offset: 0x00C (R/ )  SysTick Calibration Register
} SysTick_Type;
#endif

// SysTick control bits
#ifndef SysTick_CTRL_ENABLE_Msk
#define SysTick_CTRL_ENABLE_Msk      (1UL << 0)                // SysTick Counter Enable
#endif
#ifndef SysTick_CTRL_TICKINT_Msk
#define SysTick_CTRL_TICKINT_Msk     (1UL << 1)                // SysTick Exception Enable
#endif
#ifndef SysTick_CTRL_CLKSOURCE_Msk
#define SysTick_CTRL_CLKSOURCE_Msk   (1UL << 2)                // Clock Source: 1=CPU, 0=CPU/8
#endif

// GPIO register bit definitions
#ifndef GPIO_ODR_OD5
#define GPIO_ODR_OD5                 (1UL << 5)                // Output Data Register bit 5
#endif
#ifndef GPIO_OTYPER_OT5
#define GPIO_OTYPER_OT5              (1UL << 5)                // Output Type Register bit 5
#endif
#ifndef GPIO_OSPEEDR_OSPEED5
#define GPIO_OSPEEDR_OSPEED5         (3UL << (5 * 2))          // Output Speed Register bits for pin 5
#endif

// Add RCC clock enable definitions if missing
#ifndef RCC_IOPENR_GPIOAEN
#define RCC_IOPENR_GPIOAEN           (1UL << 0)                // GPIOA clock enable bit
#endif

// GPIO mode definitions
#ifndef GPIO_MODER_MODE5
#define GPIO_MODER_MODE5             (3UL << (5 * 2))          // Mode mask for pin 5
#endif
#ifndef GPIO_MODER_MODE5_0
#define GPIO_MODER_MODE5_0           (1UL << (5 * 2))          // Output mode for pin 5
#endif

// System Clock Configuration - Set up HSI at 48MHz
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

// Initialize GPIO pins - Configure pin A5 (LED) as output
void GPIO_Init(void)
{
    // Enable GPIOA clock
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
    
    // Configure PA5 as output (assuming it connects to an LED)
    // Use efficient bit operations: clear and set in one step
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE5) | GPIO_MODER_MODE5_0;
    
    // Set output to push-pull mode for faster switching
    GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5);
    
    // Configure for high speed operation
    GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5);
}

// SysTick-based delay function - much more efficient than busy waiting
static volatile uint32_t ms_counter = 0;

// Custom tick handler function that will be called from elsewhere
void Tick_Increment(void)
{
    ms_counter++;
}

// We don't define SysTick_Handler here since it's in startup.c
// Instead we can add a hook to be called from the ISR

void DelayMs(uint32_t ms)
{
    uint32_t start = ms_counter;
    while ((ms_counter - start) < ms);
}

// System Initialization
void SystemInit(void)
{
    // Initialize the system clock
    SystemClock_Config();
    
    // Initialize GPIO
    GPIO_Init();
    
    // Enable SysTick interrupts
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    
    // Enable global interrupts
    __enable_irq();
    
    // Call Tick_Increment every millisecond
    // Note: Since we can't modify startup.c, the user must ensure
    // that Tick_Increment is called from there
}

// Error handler
void Error_Handler(void)
{
    while(1) {
        // Infinite loop
    }
}

// Main function - Simple LED blink with optimized delay
int main(void)
{
    // System is already initialized from startup.c before main
    
    // Main loop - toggle LED (PA5) every delay interval
    while (1) {
        // Toggle LED using direct register access (more efficient)
        GPIOA->ODR ^= GPIO_ODR_OD5;
        
        // Use millisecond-based delay instead of busy-wait
        DelayMs(500);
        
        // Put CPU to sleep while waiting for next SysTick interrupt
        __WFI();
    }
    
    // Never reached
    return 0;
}