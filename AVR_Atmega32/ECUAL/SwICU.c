/*
 * SwICU.c
 *
 * Created: 29-Jan-20 2:41:19 PM
 *  Author: ahmed
 */ 
//#include "timers.h"
#include "SwICU.h"
#include "led.h"
static volatile uint8_t u8_SwICUread;
volatile uint8_t u8_setflag;

void SwICU_Init(EN_SwICU_Edge_t a_en_inputCaptureEdge){
	
	/*init timer*/
	timer0Init(T0_NORMAL_MODE,T0_OC0_DIS,T0_PRESCALER_256,0,0,T0_POLLING);
	
	/*init GPIO as input PB2--> input for INT2, GPIO as output PD0*/
	gpioPinDirection(GPIOB,BIT2,LOW);
	gpioPinDirection(GPIOD,BIT0,HIGH);
	
	/*enable INT2 irq*/
	GICR |= 0x20;
	
	/*detect rising edge write isc2>>>1*/
	MCUCSR = (a_en_inputCaptureEdge | (MCUCSR & 0xBF));	
}

void wave_send(void){
		gpioPinWrite(GPIOD,BIT0,HIGH);
		timer2DelayMs(1);
		gpioPinWrite(GPIOD,BIT0,LOW);
		
}

/*choose the detecting edge*/
void SwICU_SetCfgEdge(EN_SwICU_Edge_t a_en_inputCaptureEdge){
	
	/*set rising edge write isc2>>>1*/
	MCUCSR = (a_en_inputCaptureEdge | (MCUCSR & 0xBF));
}

/*read the ICU Value*/
void SwICU_Read(volatile uint8_t * a_pu8_capt){
	
	/*write the value here*/
	*a_pu8_capt = u8_SwICUread;
}

/*start timer*/
void SwICU_Start(void){
	
	timer0Start();
	sei();
}

void SwICU_Stop(void){
	
	timer0Stop();
}

ISR(INT2_irq){
	Led_Toggle(LED_1);
	/*
	if(MCUCSR & SwICU_EdgeRisiging){
		SwICU_Start();
		SwICU_SetCfgEdge(SwICU_EdgeFalling);
	}
	else if(!(MCUCSR & SwICU_EdgeRisiging)){
		u8_SwICUread = timer0Read();
		SwICU_SetCfgEdge(SwICU_EdgeRisiging);
		SwICU_Stop();
		u8_setflag=1;
	}*/
}