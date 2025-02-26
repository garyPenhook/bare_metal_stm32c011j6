#include <stdint.h>
#include "stm32c011xx.h"
#include "main.h"
#include "../inc/tick.h"
#include "../inc/stm32_peripherals.h"



/**
  * @brief  Main function - LED toggling using timer interrupt
  * @retval int
  */
int main(void)
{
    // Initialize system
    SystemInit();
    
    // Initialize GPIO for LED
    InitGPIO();
    
    // Initialize timer for LED toggling every 500ms
    InitTimer();
    
    // Main loop
    while (1) {
        __WFI();
    }
}