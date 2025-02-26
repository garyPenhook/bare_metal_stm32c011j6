#ifndef TICK_H
#define TICK_H

#include <stdint.h>
#include "stm32_peripherals.h"  // Add this include

// System tick counter
extern volatile uint32_t uwTick;

// External declaration - this tells other files that ms_counter exists
// somewhere else but can be used here
extern volatile uint32_t ms_counter;

// Increment system tick counter
void IncTick(void);

// Get system tick counter
uint32_t GetTick(void);

// Delay function
void Delay(uint32_t delay);

// Custom tick increment for millisecond counter
void Tick_Increment(void);

// Millisecond delay function
void DelayMs(uint32_t ms);

#endif /* TICK_H */