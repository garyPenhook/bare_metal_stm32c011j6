#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include "stm32c011xx.h"

/* System clock speed definition */
#define SYSTEM_CLOCK_HZ        48000000U   /* 48 MHz system clock */

/* Forward declarations */
void SystemInit(void);
void Reset_Handler(void);
void SystemClock_Config(void);
void GPIO_Init(void);
void Error_Handler(void);
void Delay(uint32_t count);

/* Main function */
int main(void);

#endif /* MAIN_H */