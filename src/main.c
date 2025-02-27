#include <stdint.h>
#include "stm32c011xx.h"
#include "main.h"
#include "../inc/tick.h"
#include "../inc/stm32_peripherals.h"

// Define the SystemCoreClock variable
uint32_t SystemCoreClock = 48000000U;  // Default to 48MHz

// SystemClock_Config function removed as it's already defined in system.c

/**
  * @brief  Main function - LED toggling using timer interrupt
  * @retval int
  */
int main(void)
{
    // Initialize system
    SystemInit();
    
    // Configure system clock to 48MHz
    SystemClock_Config();
    
    // Initialize GPIO for LED
    InitGPIO();
    
    // Initialize timer for LED toggling every 500ms
    InitTimer();
    
    // Main loop
    while (1) {
        __WFI();
    }
}