/*
 * PWM.c
 *
 * Created: 17-Feb-20 2:27:18 PM
 *  Author: ahmed
 */
/************************************************************************/
/*				INCLUDES                                                */
/************************************************************************/ 
#include "/Sprint projects/SOS&CarApp/Car by sos/MCAL/registers.h"
#include "/Sprint projects/SOS&CarApp/Car by sos/SERVICE/common_macros.h"
#include "/Sprint projects/SOS&CarApp/Car by sos/MCAL/PWM.h"
#include "/Sprint projects/SOS&CarApp/Car by sos/MCAL/DIO.h"

/************************************************************************/
/*						DEFINES                                         */
/************************************************************************/
#define PRECENT_TIMER (100.0)
#define MAX_DUTY	(100)
#define PRESCALE_FACTOR	(16)


/************************************************************************/
/*					GLOBAL VARIABLES                                    */
/************************************************************************/
/*flag to save if start called or not*/
static uint8_t gu8_start_flag=0;

/*prescaller values save*/
static uint8_t gu8_PWM0_Prescale =0;
static uint8_t gu8_PWM1_Prescale =0;

/************************************************************************/
/*						APIS                                            */
/************************************************************************/

/*********************************************************************************/
/* Function: Error_State Pwm_Init(Pwm_Cfg_s *Pwm_Cfg);                         */
/* Type: public                                                                  */
/* Input parameters: Pwm_Cfg Structure (channel number, Prescaler)               */
/* Return type : void                                                            */
/* Please set OC0 as output before operating here                                */
/* Description: initialize the PWM configuration                                 */
/*********************************************************************************/
ERROR_STATUS Pwm_Init(Pwm_Cfg_s *Pwm_Cfg){
	
	/*create error flag*/
	ERROR_STATUS error_flag = E_OK;
	
	/*check if pointer points to nothing then it's not ok*/
	if(Pwm_Cfg == NULL){
		
		error_flag = E_NOK;
	}
	
	/*init DIO pins OCR1A,OCR1B*/
	st_DIO_Cfg_t stOcr_pins;
	stOcr_pins.dir = OUTPUT;
	stOcr_pins.GPIO = GPIOD;
	stOcr_pins.pins = (PIN4 | PIN5);
	
	/*determine which channel to work on*/
	switch(Pwm_Cfg->Channel){
		
		/*case ch0*/
		case PWM_CH0:
		   
		   /*write the prescaler value at the global one*/
		   gu8_PWM0_Prescale = Pwm_Cfg->Prescaler;   
		   
		   /*Choose fast PWM by setting wave generation mode*/
		   SET_BIT(TCCR0,(WGM00 | WGM01));
		   
		   /*set COM01 , COM00 : Set on compare match and reset at bottom*/
		   SET_BIT(TCCR0,(COM01 | COM00));
	
		break;
		
		/*PWM1*/
		case PWM_CH1A:
			
			/*write the prescaler value at the global one*/
			gu8_PWM1_Prescale = Pwm_Cfg->Prescaler;
			
			/*set PD5/PD4 OCR1A,OCR1B as output */
			DIO_init(&stOcr_pins);
			
			/*then choose fast PWM mode using OCR1A, top = ICR1 VAL*/
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1A, WGM11 );
			SET_BIT(TCCR1B, WGM13);
			/* set on compare match and clear at bottom */
			SET_BIT(TCCR1A, COM1A0);
			SET_BIT(TCCR1A, COM1A1);
			
		break;
		
		case PWM_CH1B:
			
			/*write the prescaler value at the global one*/
			gu8_PWM1_Prescale = Pwm_Cfg->Prescaler;
			
			/*set PD5/PD4 OCR1A,OCR1B as output */
			DIO_init(&stOcr_pins);
			
			/*choose fast PWM using OCR1B, TOP = OCR1B VAL*/
			CLEAR_BIT(TCCR1A,WGM10);
			CLEAR_BIT(TCCR1B,WGM12);
			SET_BIT(TCCR1A, WGM11 );
			SET_BIT(TCCR1B, WGM13);
			/*set on compare and clear at bottom*/
			SET_BIT(TCCR1A,COM1B0);
			SET_BIT(TCCR1A,COM1B1);
			
		break;
	}
	
	/*return error status*/
	return error_flag;
}


/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: starts the PWM on the dedicated channel with the required duty   */
/*				cycle and frequency                                              */
/*-------------------------------------------------------                        */
/*(IN timer 0) there are only 5 available Frequencies:                           */
/*-------------------------------------------------------                        */
/*Frequency (NO-Prescaler) --> 31,372HZ                                          */
/*Frequency (Prescaler 8) --> 3,921HZ                                            */
/*Frequency (Prescaler 64) --> 490HZ                                             */
/*Frequency (Prescaler 256) --> 122HZ                                            */
/*Frequency (Prescaler 1024) --> 31HZ                                            */
/*--------------------------------------------------------                       */
/*(IN timer 2) there are only 7 available Frequencies:                           */
/*-------------------------------------------------------                        */
/*Frequency (NO-Prescaler) --> 31,372HZ                                          */
/*Frequency (Prescaler 8) --> 3,921HZ                                            */
/*Frequency (Prescaler 32) --> 980HZ                                             */
/*Frequency (Prescaler 64) --> 490HZ                                             */
/*Frequency (Prescaler 128) --> 245HZ                                            */
/*Frequency (Prescaler 256) --> 122HZ                                            */
/*Frequency (Prescaler 1024) --> 31HZ                                            */
/*-------------------------------------------------------                        */
/*((all this calculations are for phase correct mode))                           */
/*all this calculation for F_CPU 16MHz                                           */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Start(uint8_t Channel,uint8_t Duty,uint32_t Frequncy){
	
	/*create error status flag*/
	ERROR_STATUS error_flag = E_OK;
	
	/*set start flag*/
	gu8_start_flag = HIGH;
	
	/*init TCNT1*/
	TCNT1 = LOW;
	
	/*inverting mode*/
	Duty = MAX_DUTY-Duty;
	
	/*always use PSC =8*/
	/*set CS11 Only and clear other bits*/
	SET_BIT(TCCR1B,(CS11));
	CLEAR_BIT(TCCR1B,(CS10 | CS12));
	
	/*define ICR1 by Frequency prescaled by 8*/
	ICR1 = F_CPU/(PRESCALE_FACTOR*Frequncy);
	
	switch(Channel){
		
		/*if ch0 not implemented*/
		case PWM_CH0:
			
			/*return not okay*/
			error_flag = E_NOK;
			
		break;
		
		case PWM_CH1A:
			
			/*put the duty Value at OCR1A*/
			OCR1A = (Duty * (ICR1/PRECENT_TIMER));
			
		break;
		
		case PWM_CH1B:
		
			/*put duty val at OCR1B*/
			OCR1B = (Duty * (ICR1/PRECENT_TIMER));
			
		break;
		
		/*if CH2 then return not okay as it's not implemented*/
		case PWM_CH2:
		
			/*return not okay*/
			error_flag = E_NOK;
			
		break;
	}
	
	/*return error flag*/
	return error_flag;
		
}

/*********************************************************************************/
/* Function: Error_State Pwm_Start(uint8_t channel,uint8_t Duty);                */
/* Type: public                                                                  */
/* Input parameters: Channel ID, Duty cycle(0:100)                               */
/* Return type : Error State                                                     */
/*                                                                               */
/* Description: updates the duty cycle and frequency of the dedicated channel    */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Update(uint8_t Channel,uint8_t Duty,uint32_t Frequncy){
	
	/*create error flag*/
	ERROR_STATUS error_flag = E_OK;
	
	/*inverting mode*/
	Duty = MAX_DUTY-Duty;
	
if(gu8_start_flag ==HIGH){
	/*define ICR1 by Frequency*/
	ICR1 = F_CPU/(PRESCALE_FACTOR*Frequncy);
	
	switch(Channel){
		
		/*if ch0 not implemented*/
		case PWM_CH0:
		
			/*return not okay*/
			error_flag = E_NOK;
		
		break;
		
		case PWM_CH1A:
		
			/*put the duty Value at OCR1A*/
			OCR1A = (Duty * (ICR1/PRECENT_TIMER));
		
		break;
		
		case PWM_CH1B:
		
			/*put duty val at OCR1B*/
			OCR1B = (Duty * (ICR1/PRECENT_TIMER));
		
		break;
		
		/*if CH2 then return not okay as it's not implemented*/
		case PWM_CH2:
		
			/*return not okay*/
			error_flag = E_NOK;
			
		break;
		
		default:
			
			/*return not okay*/
			error_flag = E_NOK;
			
		break;
	}
	
	/*return Okay as things went well*/
	error_flag = E_OK;
}

	else{
	/*return not okay, start must be called first*/
	error_flag = E_NOK;
	}
	return error_flag;
}

/*********************************************************************************/
/* Function: Error_State Pwm_Stop(uint8_t channel);                              */
/* Type: public                                                                  */
/* Input parameters: channel ID                                                  */
/* Return type : Error state                                                     */
/*                                                                               */
/* Description: responsible of Stopping the PWM by clearing the prescaler		 */
/*				of the corresponding channel                                     */
/*********************************************************************************/
extern ERROR_STATUS Pwm_Stop(uint8_t Channel){
	
	/*create error_flag*/
	ERROR_STATUS error_flag = E_OK;
	
	/*reset start flag*/
	gu8_start_flag = LOW;
	
	/*check for channels*/
	switch(Channel){
		
		/*if Channel 1 then reset PSC*/	
		case PWM_CH1A:
		
			/*clear all PSC*/
			CLEAR_BIT(TCCR1B,CS12);
			CLEAR_BIT(TCCR1B,CS10);
			CLEAR_BIT(TCCR1B,CS11);
		break;
		
		case PWM_CH1B:
			
			/*clear all PSC*/
			CLEAR_BIT(TCCR1B,CS12);
			CLEAR_BIT(TCCR1B,CS10);
			CLEAR_BIT(TCCR1B,CS11);
			
		break;
		
		/*Any other channels not implemented even CH0/CH1*/
		default:
			
			error_flag = E_NOK;
		break;
	}
	
	/*return okay if code reached here*/
	return error_flag;
}