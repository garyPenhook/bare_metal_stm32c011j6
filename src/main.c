#include <stdint.h>
#include "stm32c011xx.h"
#include "main.h"
#include "../inc/tick.h"
#include "../inc/stm32_peripherals.h"

/**
  * @brief  System Clock Configuration to set HSI to 48MHz
  * @retval None
  */
void SystemClock_Config(void)
{
    /* Configure HSI to 48MHz */
    /* Reset HSIDIV bits */
    RCC->ICSCR &= ~(RCC_ICSCR_HSIDIV);
    
    /* Set HSIDIV to 0 for 48MHz 
       HSIDIV[2:0]: 000 = HSI divided by 1 (48MHz) */
    RCC->ICSCR |= (0x00 << RCC_ICSCR_HSIDIV_Pos);
    
    /* Wait for HSI to stabilize */
    while((RCC->CR & RCC_CR_HSIRDY) == 0);
    
    /* Update the SystemCoreClock variable */
    SystemCoreClockUpdate();
}

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