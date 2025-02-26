#ifndef MAIN_H
#define MAIN_H

// Add external declarations for functions defined in other files
extern void SystemInit(void);
extern void SystemClock_Config(void);
extern void Error_Handler(void);
extern void GPIO_Init(void);
extern void InitGPIO(void);
extern void InitTimer(void);

// LED definitions if needed
#define LED_PIN                  5
#define LED_PORT                GPIOA
#define TIMER_PRESCALER         4800
#define TIMER_PERIOD           10000

#endif /* MAIN_H */
