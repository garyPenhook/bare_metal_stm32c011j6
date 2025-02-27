#ifndef STM32_PERIPHERALS_H
#define STM32_PERIPHERALS_H

#include <stdint.h>
#include "stm32c011xx.h"  // Include this first to avoid redefinitions

// Define ARM Cortex-M intrinsics that might be missing
#ifndef __WFI
#define __WFI() __asm volatile ("wfi")
#endif

#ifndef __enable_irq
#define __enable_irq() __asm volatile ("cpsie i")
#endif

// NVIC Base Address and Structure (move these up)
#ifndef NVIC_BASE
#define NVIC_BASE               (0xE000E100UL)
#endif

// NVIC Structure
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

// NVIC functions (moved before SysTick_Config)
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

// SysTick definitions
#ifndef SysTick_BASE
#define SysTick_BASE            (0xE000E010UL)                  // SysTick Base Address
#endif

#ifndef SysTick_Type
typedef struct {
  volatile uint32_t CTRL;         // Offset: 0x000 (R/W)  SysTick Control and Status Register
  volatile uint32_t LOAD;         // Offset: 0x004 (R/W)  SysTick Reload Value Register
  volatile uint32_t VAL;          // Offset: 0x008 (R/W)  SysTick Current Value Register
  volatile uint32_t CALIB;        // Offset: 0x00C (R/ )  SysTick Calibration Register
} SysTick_Type;
#endif

#ifndef SysTick
#define SysTick                 ((SysTick_Type *) SysTick_BASE)  // SysTick structure pointer
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

// SysTick reload register maximum value
#ifndef SysTick_LOAD_RELOAD_Msk
#define SysTick_LOAD_RELOAD_Msk      (0xFFFFFFUL)              // SysTick Reload Register Maximum Value (24 bits)
#endif

// System core clock value (48 MHz for STM32C011)
#ifndef SystemCoreClock
// Change from #define to extern declaration
extern uint32_t SystemCoreClock;
#endif

// NVIC Priority Bits definition for STM32C011
#ifndef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS    2       // STM32C011 uses 2 priority bits
#endif

// System Control Space (SCS) -> SysTick IRQ number
#ifndef SysTick_IRQn
#define SysTick_IRQn    -1      // SysTick is a core interrupt that uses negative numbers
#endif

// SysTick Configuration Function
static inline uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk) {
    return (1UL);                                                 /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                    /* set reload register */
  NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                        /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                /* Function successful */
}

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

// RCC register definitions
// Remove duplicate definitions that are in stm32c011xx.h
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

// RCC register definitions for STM32C011xx timers
#ifndef RCC_APBENR1_TIM3EN
#define RCC_APBENR1_TIM3EN      (1UL << 1)        // TIM3 clock enable
#endif

// NVIC definitions
#ifndef TIM3_IRQn
#define TIM3_IRQn               16                // TIM3 interrupt number
#endif

#endif /* STM32_PERIPHERALS_H */
