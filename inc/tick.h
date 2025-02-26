#ifndef TICK_H
#define TICK_H

#include <stdint.h>

// System tick counter
extern volatile uint32_t uwTick;

// Increment system tick counter
void IncTick(void);

// Get system tick counter
uint32_t GetTick(void);

// Delay function
void Delay(uint32_t delay);

#endif /* TICK_H */