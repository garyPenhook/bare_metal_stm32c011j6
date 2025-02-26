#ifndef MAIN_H
#define MAIN_H

#include "stm32c011xx.h"

// Define LED pin (assuming it's on GPIOA, pin 5 - modify as needed)
#define LED_PORT GPIOA
#define LED_PIN  5

// Timer definitions
#define TIMER_PRESCALER     8000-1      // For 8 MHz system clock
#define TIMER_PERIOD        1000-1      // For 1000 ms period (500ms on, 500ms off)

// System functions
void SystemInit(void);
void SystemClock_Config(void);
void Error_Handler(void);

// GPIO functions
void GPIO_Init(void);
void InitGPIO(void);

// Timer functions
void InitTimer(void);
void TIM3_IRQHandler(void);

// Systick functions
void SysTick_Handler(void);

// Main function
int main(void);

#endif /* MAIN_H */