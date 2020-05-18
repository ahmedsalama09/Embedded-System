/*
 * led.c
 *
 * Created: 17-Jan-20 6:12:04 PM
 *  Author: ahmed
 */ 
#include "std_types.h"
#include "led.h"
#include "registers.h"
#include "gpio.h"


/*LED0---> GPIOB4
LED1---->GPIOB5
LED2---->GPIOB6
LED3----->GPIOB7*/
void Led_Init(En_LedNumber_t en_led_id){
	
	/*switch the lamb due to it's number*/
	switch(en_led_id){
		
		case LED_0:
			gpioPinDirection(GPIOB,BIT4,HIGH);
			break;
			
		case LED_1:
			gpioPinDirection(GPIOB,BIT5,HIGH);
			break;
			
		case LED_2:
			gpioPinDirection(GPIOB,BIT6,HIGH);
			break;
			
		case LED_3:
			gpioPinDirection(GPIOB,BIT7,HIGH);
			break;
	}
}

void Led_On(En_LedNumber_t en_led_id){
	
	/*switch the lamb due to it's number*/
	switch(en_led_id){
		
		case LED_0:
			gpioPinWrite(GPIOB,BIT4,HIGH);
			break;
		
		case LED_1:
			gpioPinWrite(GPIOB,BIT5,HIGH);
			break;
		
		case LED_2:
			gpioPinWrite(GPIOB,BIT6,HIGH);
			break;
		
		case LED_3:
			gpioPinWrite(GPIOB,BIT7,HIGH);
			break;
	}
}

void Led_Off(En_LedNumber_t en_led_id){
	
	/*switch the lamb due to it's number*/
	switch(en_led_id){
		
		case LED_0:
		gpioPinWrite(GPIOB,BIT4,LOW);
		break;
		
		case LED_1:
		gpioPinWrite(GPIOB,BIT5,LOW);
		break;
		
		case LED_2:
		gpioPinWrite(GPIOB,BIT6,LOW);
		break;
		
		case LED_3:
		gpioPinWrite(GPIOB,BIT7,LOW);
		break;
	}
	
}

void Led_Toggle(En_LedNumber_t en_led_id){
	
	/*switch the lamb due to it's number*/
	switch(en_led_id){
		
		case LED_0:
		gpioPinToggle(GPIOB,BIT4);
		break;
		
		case LED_1:
		gpioPinToggle(GPIOB,BIT5);
		break;
		
		case LED_2:
		gpioPinToggle(GPIOB,BIT6);
		break;
		
		case LED_3:
		gpioPinToggle(GPIOB,BIT7);
		break;
	}
	
}