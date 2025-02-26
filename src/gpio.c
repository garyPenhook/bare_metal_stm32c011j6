#include "main.h"
#include "stm32_peripherals.h"

/**
  * @brief  Initialize GPIO pins - Configure pin A5 (LED) as output
  * @retval None
  */
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

/**
  * @brief  GPIO Initialization Function for LED
  * @retval None
  */
void InitGPIO(void)
{
    // Configure LED pin as output push-pull, no pull-up, no pull-down
    LED_PORT->MODER &= ~(0x3 << (LED_PIN * 2));  // Clear mode bits
    LED_PORT->MODER |= (0x1 << (LED_PIN * 2));   // Set output mode
    
    LED_PORT->OTYPER &= ~(1 << LED_PIN);         // Push-pull mode
    LED_PORT->OSPEEDR &= ~(0x3 << (LED_PIN * 2)); // Low speed
    LED_PORT->PUPDR &= ~(0x3 << (LED_PIN * 2));  // No pull-up, no pull-down
}
