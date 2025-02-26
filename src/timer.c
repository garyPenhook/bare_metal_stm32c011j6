#include "main.h"
#include "stm32_peripherals.h"



void InitTimer(void)
{
    // Configure TIM3 for 500ms interrupts
    TIM3->PSC = TIMER_PRESCALER;  // Prescaler
    TIM3->ARR = TIMER_PERIOD;     // Auto-reload value
    
    // Enable update interrupt
    TIM3->DIER |= TIM_DIER_UIE;
    
    // Enable TIM3 interrupt in NVIC
    NVIC_SetPriority(TIM3_IRQn, 3);
    NVIC_EnableIRQ(TIM3_IRQn);
    
    // Start timer
    TIM3->CR1 |= TIM_CR1_CEN;
}

void TIM3_IRQHandler(void)
{
    // Check if update interrupt flag is set
    if (TIM3->SR & TIM_SR_UIF) {
        // Toggle the LED
        LED_PORT->ODR ^= (1 << LED_PIN);
        
        // Clear update interrupt flag
        TIM3->SR &= ~TIM_SR_UIF;
    }
}
