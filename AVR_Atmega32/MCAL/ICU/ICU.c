/*
 * ICU.c
 *
 * Created: 18-Feb-20 10:24:05 AM
 *  Author: ahmed
 */ 

/************************************************************************/
/*				INCLUDES                                                */
/************************************************************************/
#include "/Sprint projects/SOS&CarApp/Car by sos/ECUAL/ICU.h"
#include "/Sprint projects/SOS&CarApp/Car by sos/MCAL/timer.h"
#include "/Sprint projects/SOS&CarApp/Car by sos/MCAL/DIO.h"
#include "/Sprint projects/SOS&CarApp/Car by sos/MCAL/interrupt.h"
#include "/Sprint projects/SOS&CarApp/Car by sos/MCAL/registers.h"


/************************************************************************/
/*							Defines                                     */
/************************************************************************/
#define RISING_EDGE (1)
#define FALLING_EDGE (0)
#define TICK_TIME   (64)
#define TCNT_0_VAL		(0)
#define ICU_NOT_INIT	(0)
#define ICU_INIT		(1)


/************************************************************************/
/*							Global Vars                                 */
/************************************************************************/
static uint8_t gu8_IcuState = ICU_NOT_INIT;
static volatile uint8_t gu8_ICU_FlagState = ZERO;

/************************************************************************/
/*								APIS                                    */
/************************************************************************/
/**************************************************************************
 * Function 	: Icu_Init                                                *
 * Input 		: Icu_cfg : pointer to structure of type Icu_cfg_s        *
 * Return 		: value of type ERROR_STATUS							  *
 * 				  which is one of the following values:					  *
 * 				  - E_OK  : initialized successfully					  *
 *				  - E_NOK : not initialized successfully				  *
 * Description  : Initializes the ICU by initializing the timer			  *
 * 				  and enabling the global interrupt						  *
 **************************************************************************/

/*global variables*/
static uint8_t gu8_second_edge;
static volatile uint8_t gu8_int2_flag=LOW;
static volatile uint8_t gu8_wave_received = LOW;

ERROR_STATUS Icu_Init(Icu_cfg_s * Icu_Cfg){
	
	/*Create error status Variable*/
	ERROR_STATUS error_flag = E_OK;
	
	/*check if the pointer pointing to struct or null*/
	if(Icu_Cfg == NULL){
		
		/*return not ok*/
		error_flag = ERROR_ICU_BASE + ERROR_NULL_PTR_INIT;
	}
	
	/*pointer isn't null then proceed*/
	else{

		/*check if ICU initialized before or not*/
		if(gu8_IcuState == ICU_NOT_INIT)
		{
			/*check the channels*/
		switch(Icu_Cfg->ICU_Ch_No){
			
			/*FOR CH0,CH1 it's not implemented*/
			case ICU_CH0:
				
				/*Error_Not_implemented*/
				error_flag = ERROR_ICU_BASE + ERROR_CHANNEL_NOT_IMPLEMENTED;			
			break;
			
			/*FOR CH1 it's not implemented*/
			case ICU_CH1:
			
				/*Error Not implemented*/
				error_flag = ERROR_ICU_BASE + ERROR_CHANNEL_NOT_IMPLEMENTED;
			break;
			
			/*for CH2 it's on PINB2*/
			case ICU_CH2:
				
				/*enable irq2 bit at GICR*/
				SET_BIT(GICR,INT2_BIT);
				SET_BIT(MCUCSR,ISC2_BIT);
				
				/*check timers and notice only TIM0 used*/
				switch(Icu_Cfg->ICU_Ch_Timer){
					
					case ICU_TIMER_CH0:
						
						/*set PSC value with 256*/
						TCCR0 = PSC_1024_TM0;
						TCNT0  = LOW;

						/*set init as high*/
						gu8_IcuState = ICU_INIT;
						
					break;
					
					case ICU_TIMER_CH1:
						
						/*not implemented*/
						error_flag = ERROR_ICU_BASE + ERROR_TIMER_NOT_IMPLEMENTED;
					break;
					
					case ICU_TIMER_CH2:
						
						/*set PSC value with 256*/
						TCCR2 = PSC_1024_TM2;
						TCNT2  = LOW;

						/*set init as high*/
						gu8_IcuState = ICU_INIT;
					break;
					
				default:
						
						/*wrong channel picked up Error*/
						error_flag = ERROR_ICU_BASE + ERROR_TIMER_NOT_IMPLEMENTED;
				break;
				}
			}
		
		}

	else
		{
		/* Save error multiple initialization */

		error_flag = ERROR_ICU_BASE + ERROR_MULTIPLE_MODULE_INIT;
		}
	}
		
		
	/*return error flag*/
	return error_flag;
}

/***************************************************************************
 * Function		: Icu_ReadTime
 * Input		: 
 *				Icu_Channel --> ICU timer channel
 *									- ICU_TIMER_CH0
 *									- ICU_TIMER_CH1
 *									- ICU_TIMER_CH2
 *				Icu_EdgeToEdge -- > edges to calculate pusle time between:
 *									- ICU_RISE_TO_RISE
 *									- ICU_RISE_TO_FALL
 *									- ICU_FALE_TO_RISE					   *
 * Output 		: Icu_Time : pointer to uint32 variable to give the time   *
 * 				  from falling edge to rising edge						   *
 * Return 		: value of type ERROR_STATUS							   *
 * 				  which is one of the following values:					   *
 * 				  - E_OK : successful									   *
 *				  - E_NOK : not successful								   *
 * Description	: calculates the time between 2 edges				       *
 ***************************************************************************/
ERROR_STATUS Icu_ReadTime(uint8_t Icu_Channel, uint8_t Icu_EdgeToEdge, uint32_t * Icu_Time){
	
	
	//DIO_Cfg_s st_irq_dio_t;
	st_DIO_Cfg_t st_DioIcu_t;

	/*create error flag*/
	ERROR_STATUS error_flag = E_OK;
	
	if(Icu_Time == NULL){

		error_flag = ERROR_ICU_BASE + ERROR_NULL_PTR_INIT;
	}

	else{

	/*check if channel is init or not*/
	if(gu8_IcuState == ICU_INIT)
	{
		/*pick up channel*/
	switch(Icu_Channel){
	
		
		case ICU_CH2:

		st_DioIcu_t.dir = INPUT;
		st_DioIcu_t.GPIO = GPIOB;
		st_DioIcu_t.pins = PIN2;
		DIO_init(&st_DioIcu_t);

			 /*CHOOSE Starting Edge*/
			 switch(Icu_EdgeToEdge){
				 
				 /*for rising to rising then both edges of INT2 detection is rising edge*/
				 case ICU_RISE_TO_RISE:
					
					/*first edge is rising*/
					SET_BIT(MCUCSR,ISC2_BIT);
					
					/*clear INT2 flags*/
					SET_BIT(GIFR,INTF2_FLAG);
					
					/*choose the second edge type*/
					gu8_second_edge = RISING_EDGE;
					
					/*enable global irq*/
					sei();
					
				break;
				
				/*for rising to falling then first is rising and second is falling*/
				case ICU_RISE_TO_FALL:
				
					/*first edge is rising*/
					SET_BIT(MCUCSR,ISC2_BIT);
					
					/*clear INT2 flags*/
					SET_BIT(GIFR,INTF2_FLAG);
					
					/*choose the second edge type*/
					gu8_second_edge = FALLING_EDGE;
					
					/*enable global irq*/
					sei();
					
				break;
					
				/*for falling to rising then first is falling second detecting is rising*/
				case ICU_FALE_TO_RISE:
				
					/*first edge is falling*/
					CLEAR_BIT(MCUCSR,ISC2_BIT);
					
					/*clear INT2 FLAG*/
					SET_BIT(GIFR,INTF2_FLAG);
					
					/*choose the second edge type*/
					gu8_second_edge = RISING_EDGE;
					
					/*enable global irq*/
					sei();
					
				break;
				
				/*if any other value is entered then it's not ok*/
				default:
					error_flag = ERROR_ICU_BASE + ERROR_INVLAID_VALUE;
				break;
			 }
			while(gu8_wave_received != HIGH)
			;
				
			 /*reset all flag*/
			 gu8_wave_received = LOW;
			 gu8_int2_flag = LOW;
			 
			 /*return the wave time = TICK_TIME * value in counter when the wave is received*/
			 *Icu_Time = (uint32_t) TICK_TIME * TCNT2;
			 
			 break;
			 
	default:

			error_flag = ERROR_ICU_BASE + ERROR_CHANNEL_NOT_IMPLEMENTED;
	break;
		}
	}

	else
		{
		/* Module not init */
		error_flag = ERROR_ICU_BASE + ERROR_MODULE_NOT_INIT;
		}
	}
	return error_flag;
}


ERROR_STATUS ICU_ISR_Status(uint8_t *ISR_Status){
	
	if(gu8_ICU_FlagState == 1)
	{
		/*create error flag*/
		ERROR_STATUS ERR_STATE = E_OK;
		
		*ISR_Status = gu8_ICU_FlagState;
		
		gu8_ICU_FlagState = 0;
	}
	return ERR_STATE;
}

ISR(INT2_irq){
		
		gu8_ICU_FlagState = 1;
	
		/*check for first edge flag*/
		if(gu8_int2_flag == LOW){
			
			/*init TCNT0*/
			TCNT2 = TCNT_0_VAL;
			
			/*write the second edge detection*/
			if(gu8_second_edge == RISING_EDGE){
				SET_BIT(MCUCSR,ISC2_BIT);
			}
			
			/*check if 2nd edge is falling edge*/
			else if (gu8_second_edge == FALLING_EDGE){
				CLEAR_BIT(MCUCSR,ISC2_BIT);
			}
			
			/*set first edge flag HIGH*/
			gu8_int2_flag = HIGH;
		}
		/*if the flag is high then fall wave is received set wave received flag*/
		else{
			
			/*set wave received as high*/
			gu8_wave_received = HIGH;
		}
}
