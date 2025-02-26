#include "../inc/stm32_peripherals.h"  // Include this first
#include "tick.h"
#include "stm32c011xx.h"

// Global variables
volatile uint32_t uwTick = 0;
volatile uint32_t ms_counter = 0;

/**
  * @brief  SysTick handler
  * @retval None
  */
void SysTick_Handler(void)
{
    ms_counter++;
}

/**
  * @brief  Increment tick counter
  * @retval None
  */
void IncTick(void)
{
    uwTick++;
}

/**
  * @brief  Get tick counter
  * @retval Tick counter value
  */
uint32_t GetTick(void)
{
    return uwTick;
}

/**
  * @brief  Delay function
  * @param  delay: delay in milliseconds
  * @retval None
  */
void Delay(uint32_t delay)
{
    uint32_t tickstart = GetTick();
    while((GetTick() - tickstart) < delay);
}

/**
  * @brief  Custom tick increment function for millisecond counter
  * @retval None
  */
void Tick_Increment(void)
{
    ms_counter++;
}

/**
  * @brief  Millisecond delay function using the ms_counter
  * @param  ms: delay in milliseconds
  * @retval None
  */
void DelayMs(uint32_t ms)
{
    uint32_t start = ms_counter;
    while ((ms_counter - start) < ms);
}

void tick_init(void) {
    // Configure SysTick for 1ms interrupts
    SysTick_Config(SystemCoreClock / 1000);
}

uint32_t get_tick_ms(void) {
    return ms_counter;
}

void delay_ms(uint32_t ms) {
    uint32_t start = ms_counter;
    while ((ms_counter - start) < ms) {
        // Wait
    }
}
