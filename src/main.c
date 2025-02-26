#include <stdint.h>
#include "stm32c011xx.h"
#include "main.h"
#include "../inc/tick.h" // Fix the path to tick.h

// Define ARM Cortex-M intrinsics that might be missing
// Add at the top of your file if stdint.h isn't working

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

// Add missing timer definitions
// Timer register base addresses for STM32C011xx
#ifndef TIM3_BASE
#define TIM3_BASE               (0x40000400UL)
#endif

#ifndef TIM3
#define TIM3                    ((TIM_TypeDef *) TIM3_BASE)
#endif

// Timer register structure
#ifndef TIM_TypeDef
typedef struct {
  volatile uint32_t CR1;         // Control register 1
  volatile uint32_t CR2;         // Control register 2
  volatile uint32_t SMCR;        // Slave mode control register
  volatile uint32_t DIER;        // DMA/Interrupt enable register
  volatile uint32_t SR;          // Status register
  volatile uint32_t EGR;         // Event generation register
  volatile uint32_t CCMR1;       // Capture/compare mode register 1
  volatile uint32_t CCMR2;       // Capture/compare mode register 2
  volatile uint32_t CCER;        // Capture/compare enable register
  volatile uint32_t CNT;         // Counter
  volatile uint32_t PSC;         // Prescaler
  volatile uint32_t ARR;         // Auto-reload register
  volatile uint32_t RCR;         // Repetition counter register
  volatile uint32_t CCR1;        // Capture/compare register 1
  volatile uint32_t CCR2;        // Capture/compare register 2
  volatile uint32_t CCR3;        // Capture/compare register 3
  volatile uint32_t CCR4;        // Capture/compare register 4
  volatile uint32_t BDTR;        // Break and dead-time register
  volatile uint32_t DCR;         // DMA control register
  volatile uint32_t DMAR;        // DMA address for full transfer
  volatile uint32_t OR;          // Option register
} TIM_TypeDef;
#endif

// Timer register bit definitions
#ifndef TIM_CR1_CEN
#define TIM_CR1_CEN             (1UL << 0)        // Counter enable
#endif

#ifndef TIM_DIER_UIE
#define TIM_DIER_UIE            (1UL << 0)        // Update interrupt enable
#endif

#ifndef TIM_SR_UIF
#define TIM_SR_UIF              (1UL << 0)        // Update interrupt flag
#endif

// RCC register definitions for STM32C011xx
#ifndef RCC_APBENR1_TIM3EN
#define RCC_APBENR1_TIM3EN      (1UL << 1)        // TIM3 clock enable
#endif

// NVIC definitions
#ifndef TIM3_IRQn
#define TIM3_IRQn               16                // TIM3 interrupt number
#endif

// Define NVIC structure if it's not already defined
#ifndef NVIC_BASE
#define NVIC_BASE               (0xE000E100UL)    // NVIC Base Address
#endif

// Define the NVIC_Type structure
typedef struct {
  volatile uint32_t ISER[8];     // Interrupt Set Enable Register
  uint32_t RESERVED0[24];
  volatile uint32_t ICER[8];     // Interrupt Clear Enable Register
  uint32_t RESERVED1[24];
  volatile uint32_t ISPR[8];     // Interrupt Set Pending Register
  uint32_t RESERVED2[24];
  volatile uint32_t ICPR[8];     // Interrupt Clear Pending Register
  uint32_t RESERVED3[24];
  volatile uint32_t IABR[8];     // Interrupt Active bit Register
  uint32_t RESERVED4[56];
  volatile uint8_t IP[240];      // Interrupt Priority Register (8Bit wide)
  uint32_t RESERVED5[644];
  volatile uint32_t STIR;        // Software Trigger Interrupt Register
} NVIC_Type;

#ifndef NVIC
#define NVIC                    ((NVIC_Type *) NVIC_BASE)
#endif

// NVIC functions
static inline void NVIC_SetPriority(int32_t IRQn, uint32_t priority)
{
  if(IRQn >= 0) {
    NVIC->IP[IRQn] = (uint8_t)((priority << 4) & 0xff);
  }
}

static inline void NVIC_EnableIRQ(int32_t IRQn)
{
  if(IRQn >= 0) {
    NVIC->ISER[0] = (1UL << ((uint32_t)IRQn & 0x1F));
  }
}

// Global variables
volatile uint32_t uwTick = 0;

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

// Custom tick handler function that will be called from elsewhere
static volatile uint32_t ms_counter = 0;

void Tick_Increment(void)
{
    ms_counter++;
}

void DelayMs(uint32_t ms)
{
    uint32_t start = ms_counter;
    while ((ms_counter - start) < ms);
}

// System Initialization
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

/**
  * @brief  GPIO Initialization Function
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

/**
  * @brief  Timer Configuration for periodic interrupt
  * @retval None
  */
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

/**
  * @brief  This function handles TIM3 global interrupt.
  * @retval None
  */
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

/**
  * @brief  SysTick handler
  * @retval None
  */
void SysTick_Handler(void)
{
    IncTick();
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

// Main function - Simple LED blink with optimized delay
int main(void)
{
    // Initialize system
    SystemInit();
    
    // Initialize GPIO
    InitGPIO();
    
    // Initialize timer for LED toggling
    InitTimer();
    
    // Main loop
    while (1) {
        // No need to manually toggle the LED
        // The timer interrupt will handle it
    }
}