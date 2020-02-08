/*
 * HwPWM.c
 *
 * Created: 30-Jan-20 5:04:48 PM
 *  Author: ahmed
 */ 
#include "HwPWM.h"

/* Bits Macros */

#define WGM10		0
#define WGM11		1
#define WGM12		3
#define WGM13		4
#define FOC1B		2
#define FOC1A		3

#define COM1A0		6
#define COM1A1		7

#define COM1B0		4
#define COM1B1		5

#define CS10		0
#define CS11		1
#define CS12		2

#define ICES1		6
#define ICNC1		7


void HwPWMInit(void)
{
	/* set OC1A and OC1B pins as output pins */
	gpioPinDirection(GPIOD, BIT4, OUTPUT);
	gpioPinDirection(GPIOD, BIT5, OUTPUT);
	
	/* first enable PWM mode */
	CLEAR_BIT(TCCR1A, FOC1B);
	CLEAR_BIT(TCCR1A, FOC1A);

	/*then choose fast PWM mode using OCR1A */
	CLEAR_BIT(TCCR1A, WGM10);
	SET_BIT(TCCR1A, WGM11);
	
	SET_BIT(TCCR1B, WGM12);
	SET_BIT(TCCR1B, WGM13);

	/* use OCR1A and OCR1B */
	CLEAR_BIT(TCCR1A, COM1A0);
	SET_BIT(TCCR1A, COM1A1);
	
	CLEAR_BIT(TCCR1A, COM1B0);
	SET_BIT(TCCR1A, COM1B1);

	/* clear input capture bits */
	CLEAR_BIT(TCCR1B, ICES1);
	CLEAR_BIT(TCCR1B, ICNC1);

	/* start counting from 0 */
	TCNT1 = 0;
}

void HwPWMSetDuty(uint8_t a_u8_duty, uint32_t a_u32_frequency)
{
	if(a_u32_frequency >= 245)
	{
		ICR1 = F_CPU / (1UL * a_u32_frequency);

		/* set clock for the timer */
		TCCR1B |= T1_PRESCALER_NO;
	}else if(a_u32_frequency >= 31)
	{
		ICR1 = F_CPU / (8UL * a_u32_frequency);

		/* set clock for the timer */
		TCCR1B |= T1_PRESCALER_8;
	}

	OCR1A = a_u8_duty * (ICR1 / 100);
	OCR1B = a_u8_duty * (ICR1 / 100);
	
}
