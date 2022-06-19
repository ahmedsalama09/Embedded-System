/*
 * interrupt.h
 *
 *  Created on: Dec 9, 2019
 *      Author: ahmad
 */



#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "registers.h"



#define INTO_irq			__vector_1
#define INT1_irq			__vector_2
#define INT2_irq			__vector_3
#define TIMER2_COMP_irq		__vector_4
#define TIMER2_OVF_irq		__vector_5
#define TIMER1_CAPT_irq		__vector_6
#define TIMER1_COMPA_irq	__vector_7
#define TIMER1_COMPB_irq	__vector_8
#define TIMER1_OVF_irq		__vector_9
#define TIMER0_COMP_irq		__vector_10
#define TIMER0_OVF_irq		__vector_11
#define SPI_STC_irq			__vector_12
#define USART_RXC_irq		__vector_13
#define USART_UDRE_irq		__vector_14
#define USART_TXC_irq		__vector_15
#define ADC_irq				__vector_16
#define EE_RDY_irq			__vector_17
#define ANA_COMP_irq		__vector_18
#define TWI_irq				__vector_19
#define SPM_RDY_irq			__vector_20

#define ISR(vector)            \
   void vector (void) __attribute__ ((interrupt)); \
   void vector (void)

#define sei()  __asm__ __volatile__ ("sei" ::)
#define cli()  __asm__ __volatile__ ("cli" ::)

#endif /* INTERRUPT_H_ */
