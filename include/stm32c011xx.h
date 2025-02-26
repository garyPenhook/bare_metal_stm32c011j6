#ifndef STM32C011XX_H
#define STM32C011XX_H

#include <stdint.h>

/* IO definitions */
#define __IO volatile
#define __I volatile const

/* Base addresses of memories */
#define FLASH_BASE      (0x08000000UL) /* Flash base address */
#define SRAM_BASE       (0x20000000UL) /* SRAM base address */
#define PERIPH_BASE     (0x40000000UL) /* Peripheral base address */

/* Base addresses of peripherals */
#define APB1PERIPH_BASE (PERIPH_BASE)
#define APB2PERIPH_BASE (PERIPH_BASE + 0x00010000UL)
#define AHBPERIPH_BASE  (PERIPH_BASE + 0x00020000UL)
#define IOPORT_BASE     (PERIPH_BASE + 0x10000000UL)

/* GPIO Peripheral base addresses */
#define GPIOA_BASE      (IOPORT_BASE + 0x00000000UL)
#define GPIOB_BASE      (IOPORT_BASE + 0x00000400UL)
#define GPIOC_BASE      (IOPORT_BASE + 0x00000800UL)
#define GPIOF_BASE      (IOPORT_BASE + 0x00001C00UL)

/* RCC Peripheral base address */
#define RCC_BASE        (AHBPERIPH_BASE + 0x00001000UL)

/* GPIO register structure */
typedef struct {
    __IO uint32_t MODER;       /* Mode register,                   Address offset: 0x00 */
    __IO uint32_t OTYPER;      /* Output type register,            Address offset: 0x04 */
    __IO uint32_t OSPEEDR;     /* Output speed register,           Address offset: 0x08 */
    __IO uint32_t PUPDR;       /* Pull-up/pull-down register,      Address offset: 0x0C */
    __IO uint32_t IDR;         /* Input data register,             Address offset: 0x10 */
    __IO uint32_t ODR;         /* Output data register,            Address offset: 0x14 */
    __IO uint32_t BSRR;        /* Bit set/reset register,          Address offset: 0x18 */
    __IO uint32_t LCKR;        /* Configuration lock register,     Address offset: 0x1C */
    __IO uint32_t AFR[2];      /* Alternate function registers,    Address offset: 0x20-0x24 */
    __IO uint32_t BRR;         /* Bit reset register,              Address offset: 0x28 */
} GPIO_TypeDef;

/* RCC register structure */
typedef struct {
    __IO uint32_t CR;          /* Clock control register,          Address offset: 0x00 */
    __IO uint32_t ICSCR;       /* Internal clock sources calibration register, Address offset: 0x04 */
    __IO uint32_t CFGR;        /* Clock configuration register,    Address offset: 0x08 */
    __IO uint32_t PLLCFGR;     /* PLL configuration register,      Address offset: 0x0C */
    uint32_t      RESERVED0;   /* Reserved,                        Address offset: 0x10 */
    uint32_t      RESERVED1;   /* Reserved,                        Address offset: 0x14 */
    __IO uint32_t CIER;        /* Clock interrupt enable register, Address offset: 0x18 */
    __IO uint32_t CIFR;        /* Clock interrupt flag register,   Address offset: 0x1C */
    __IO uint32_t CICR;        /* Clock interrupt clear register,  Address offset: 0x20 */
    uint32_t      RESERVED2;   /* Reserved,                        Address offset: 0x24 */
    __IO uint32_t AHBENR;      /* AHB peripheral clock enable register, Address offset: 0x28 */
    __IO uint32_t APBENR1;     /* APB peripheral clock enable register 1, Address offset: 0x2C */
    __IO uint32_t APBENR2;     /* APB peripheral clock enable register 2, Address offset: 0x30 */
    uint32_t      RESERVED3;   /* Reserved,                        Address offset: 0x34 */
    __IO uint32_t IOPENR;      /* GPIO clock enable register,      Address offset: 0x38 */
    uint32_t      RESERVED4;   /* Reserved,                        Address offset: 0x3C */
    __IO uint32_t AHBRSTR;     /* AHB peripheral reset register,   Address offset: 0x40 */
    __IO uint32_t APBRSTR1;    /* APB peripheral reset register 1, Address offset: 0x44 */
    __IO uint32_t APBRSTR2;    /* APB peripheral reset register 2, Address offset: 0x48 */
    uint32_t      RESERVED5;   /* Reserved,                        Address offset: 0x4C */
    __IO uint32_t IOPRSTR;     /* GPIO reset register,             Address offset: 0x50 */
} RCC_TypeDef;

/* Peripheral pointers */
#define GPIOA               ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOF               ((GPIO_TypeDef *)GPIOF_BASE)
#define RCC                 ((RCC_TypeDef *)RCC_BASE)

/* Bit definitions */
/* RCC IOPENR */
#define RCC_IOPENR_GPIOAEN_Pos     (0U)
#define RCC_IOPENR_GPIOAEN_Msk     (0x1UL << RCC_IOPENR_GPIOAEN_Pos)
#define RCC_IOPENR_GPIOAEN         RCC_IOPENR_GPIOAEN_Msk
#define RCC_IOPENR_GPIOBEN_Pos     (1U)
#define RCC_IOPENR_GPIOBEN_Msk     (0x1UL << RCC_IOPENR_GPIOBEN_Pos)
#define RCC_IOPENR_GPIOBEN         RCC_IOPENR_GPIOBEN_Msk
#define RCC_IOPENR_GPIOCEN_Pos     (2U)
#define RCC_IOPENR_GPIOCEN_Msk     (0x1UL << RCC_IOPENR_GPIOCEN_Pos)
#define RCC_IOPENR_GPIOCEN         RCC_IOPENR_GPIOCEN_Msk
#define RCC_IOPENR_GPIOFEN_Pos     (5U)
#define RCC_IOPENR_GPIOFEN_Msk     (0x1UL << RCC_IOPENR_GPIOFEN_Pos)
#define RCC_IOPENR_GPIOFEN         RCC_IOPENR_GPIOFEN_Msk

/* GPIO Mode register */
#define GPIO_MODER_MODE0_Pos       (0U)
#define GPIO_MODER_MODE0_Msk       (0x3UL << GPIO_MODER_MODE0_Pos)
#define GPIO_MODER_MODE0           GPIO_MODER_MODE0_Msk
#define GPIO_MODER_MODE0_0         (0x1UL << GPIO_MODER_MODE0_Pos)
#define GPIO_MODER_MODE0_1         (0x2UL << GPIO_MODER_MODE0_Pos)

#define GPIO_MODER_MODE5_Pos       (10U)
#define GPIO_MODER_MODE5_Msk       (0x3UL << GPIO_MODER_MODE5_Pos)
#define GPIO_MODER_MODE5           GPIO_MODER_MODE5_Msk
#define GPIO_MODER_MODE5_0         (0x1UL << GPIO_MODER_MODE5_Pos)
#define GPIO_MODER_MODE5_1         (0x2UL << GPIO_MODER_MODE5_Pos)

/* GPIO BSRR register bits */
#define GPIO_BSRR_BS5_Pos          (5U)
#define GPIO_BSRR_BS5_Msk          (0x1UL << GPIO_BSRR_BS5_Pos)
#define GPIO_BSRR_BS5              GPIO_BSRR_BS5_Msk

#define GPIO_BSRR_BR5_Pos          (21U)
#define GPIO_BSRR_BR5_Msk          (0x1UL << GPIO_BSRR_BR5_Pos)
#define GPIO_BSRR_BR5              GPIO_BSRR_BR5_Msk

/* GPIO modes */
#define GPIO_MODE_INPUT            0x00000000U
#define GPIO_MODE_OUTPUT_PP        0x00000001U
#define GPIO_MODE_AF               0x00000002U
#define GPIO_MODE_ANALOG           0x00000003U

#endif /* STM32C011XX_H */