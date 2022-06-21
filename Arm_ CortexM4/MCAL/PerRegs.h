/*Author : A7md Salama
*/
#ifndef PERREGS_H
#define PERREGS_H
asdads
#ifdef  PERREGS_H
asd
/*includes*/
#include "../SERVICES/std_types.h"

/*Adrs definitions*/
#define RCC_BASE (0x40023800)

/*GPIO Base Addrs*/
#define GPIOA_BASE (0x40020000)
#define GPIOB_BASE (0x40020400)
#define GPIOC_BASE (0x40020800)
#define GPIOD_BASE (0x40020C00)
#define GPIOE_BASE (0x40021000)
#define GPIOH_BASE (0x40021C00)

/*ADC Base Addr*/
#define ADC1_BASE (0x40012000)
#define ADC2_BASE (0x40012100)
#define ADC3_BASE (0x40012200)

/*Common ADC Reg*/
#define ADC_BASE  (0x40012300)

/*I2C Base Addr*/
#define I2C1_BASE (0x40005400)
#define I2C2_BASE (0x40005800)
#define I2C3_BASE (0x40005C00)

/*ADC Clock Macros*/
#define RCC_APB2ENR_ADC1CK_EN (256)
#define RCC_APB2ENR_ADC2CK_EN (512)
#define RCC_APB2ENR_ADC3CK_EN (1024)

/*ADC Arrays Index Macros*/
#define SMPR1 (0)
#define SMPR2 (1)

#define SQR1 (0)
#define SQR2 (1)
#define SQR3 (2)

#define JOFR1 (0)
#define JOFR2 (1)
#define JOFR3 (2)
#define JOFR4 (3)

/*Rcc Regs*/
typedef struct
{
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t reserved1;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t reserved2;
    volatile uint32_t reserved3;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t reserved4;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t reserved5;
    volatile uint32_t reserved6;
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile uint32_t AHB3LPENR;
    volatile uint32_t reserved7;
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    volatile uint32_t reserved8;
    volatile uint32_t reserved9;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t reserved10;
    volatile uint32_t reserved11;
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    volatile uint32_t PLLSAICFGR;
    volatile uint32_t DCKCFGR;
}str_RCC_t;

/*Gpio Regs*/
typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile const uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint16_t BSRRL;
    volatile uint16_t BSRRH;
    volatile uint32_t LCKR;
    volatile uint8_t AFR[8];
}str_GPIO_t;


/*ADC Regs*/
typedef struct
{
	volatile uint32_t SR;
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SMPR[2];
	volatile uint32_t JOFR[4];
	volatile uint32_t HTR;
	volatile uint32_t LTR;
	volatile uint32_t SQR[3];
	volatile uint32_t JSQR;
	volatile uint32_t JDR[4];
	volatile uint32_t DR;
}str_ADCCH_t;

/*Common ADC Regs*/
typedef struct
{
	volatile uint32_t CSR;
	volatile uint32_t CDR;
}str_ADC_t;

/*I2C Regs*/
typedef struct
{
	volatile uint32_t CR1;
	volatile uint16_t CR2 :14;
	volatile uint16_t Reserved1;
	volatile uint16_t OAR1;
	volatile uint16_t Reserved2;
	volatile uint16_t OAR2 :8;
	volatile uint16_t Reserved3;
	volatile uint16_t DR : 8;
	volatile uint16_t Reserved4;
	volatile uint16_t SR1;
	volatile uint16_t Reserved5;
	volatile uint16_t SR2;
	volatile uint16_t Reserved6;
	volatile uint16_t CCR;
	volatile uint16_t Reserved7;
	volatile uint16_t TRISE : 6;
	volatile uint16_t Reserved8;
	volatile uint16_t FLTR : 5;
	volatile uint16_t Reserved9;
}str_I2C_t;

/*Define pointer to RCC*/
#define RCC ((str_RCC_t *) RCC_BASE)

/*this type to be indicator of constant pointer to GPIO struct*/
typedef volatile str_GPIO_t * GPIO_PTR;

/*Define GPIO Adress*/
#define GPIOA ((str_GPIO_t *) GPIOA_BASE)
#define GPIOB ((str_GPIO_t *) GPIOB_BASE)
#define GPIOC ((str_GPIO_t *) GPIOC_BASE)
#define GPIOD ((str_GPIO_t *) GPIOD_BASE)
#define GPIOE ((str_GPIO_t *) GPIOE_BASE)
#define GPIOH ((str_GPIO_t *) GPIOH_BASE)

/*Define ADC Address*/
#define ADC1 ((str_ADCCH_t *) ADC1_BASE)
#define ADC2 ((str_ADCCH_t *) ADC2_BASE)
#define ADC3 ((str_ADCCH_t *) ADC3_BASE)

/*I2C Defines*/
#define I2C1 ((str_I2C_t *) I2C1_BASE)
#define I2C2 ((str_I2C_t *) I2C2_BASE)
#define I2C3 ((str_I2C_t *) I2C3_BASE)


#endif
#endif
