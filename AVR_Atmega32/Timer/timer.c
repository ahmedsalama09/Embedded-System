
  
/*
 * timer.c
 *
 * Created: 20-Jan-20 4:57:25 PM
 *  Author: ahmed
 */ 

/************************************************************************/
/*						INCLUDES                                        */
/************************************************************************/
#include "timer.h"
#include "registers.h"
#include "/Sprint projects/SOS&CarApp/Car by sos/SERVICE/common_macros.h"
#include "interrupt.h"

/************************************************************************/
/*					DEFINES                                             */
/************************************************************************/
#define OVF_FLAG_SET	(1)
#define OVF_FLAG_CLEAR	(0)

/************************************************************************/
/*				GLOBAL VARIABLES                                        */
/************************************************************************/
static uint8_t gu8_prescalerVal_Timer0;
static uint8_t gu8_prescalerVal_Timer1;
static uint8_t gu8_prescalerVal_Timer2;
typedef void vvoid;
volatile static vvoid(*ptr_callback)(void);

/************************************************************************/
/*							APIS                                        */
/************************************************************************/
/**
 * Input: Pointer to a structure contains the information needed to initialize the timer. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module
 * 							
 */
ERROR_STATUS Timer_Init(Timer_cfg_s *st_Timer_cfg_t)
{
		/*create error flag*/
		ERROR_STATUS  error_flag = E_OK;
		
		/*first init the structure and check if it's done or not*/
		if(st_Timer_cfg_t == NULL){
			
			/*return FAILED STATUS*/
			error_flag = E_NOK;
		}
		else{
		
		/*check for the channel number*/
		switch(st_Timer_cfg_t->Timer_CH_NO){
			
			/*if it's timer 0*/
			case TIMER_CH0:
			
			    /*check if falling edge was chosen*/
			    if(st_Timer_cfg_t->Timer_Mode == COUNTER_FALLING_MODE){
				
				   /*Choose Falling Edge counter*/
				   SET_BIT(TCCR0,CS01);
				   SET_BIT(TCCR0,CS02);
			   }
			   else if(st_Timer_cfg_t->Timer_Mode == COUNTER_RISING_MODE){
				
				   /*Choose Rising Edge Counter*/
				   SET_BIT(TCCR0,CS00);
				   SET_BIT(TCCR0,CS02);
				   SET_BIT(TCCR0,CS01);
			   }

			   /*check if internal clock is clock was chosen*/
			   else if (st_Timer_cfg_t->Timer_Mode == TIMER_MODE){
				
				   /*write the prescaler*/
				   gu8_prescalerVal_Timer0 = st_Timer_cfg_t->Timer_Prescaler;
			   }
			
			   /*enable interrupt if it's chosen*/
			   if(st_Timer_cfg_t->Timer_INT_Mode == TIMER_INTERRUPT_MODE){
				
				   /*write 1 at TIMSK*/
				   TIMSK = ((TIMSK & TIM0_INTERRUPT_MASK) | TIM0_INTERRUPT_EN);
				   
				   /*set callback Val*/
				   ptr_callback = (volatile void *)st_Timer_cfg_t->Timer_Cbk_ptr;
				   
				   	sei();
			   }
			   /*enable interrupt if it's chosen*/
			   else if(st_Timer_cfg_t->Timer_INT_Mode == TIMER_POLLING_MODE){
				
				   /*write 0 at TIMSK*/
				   TIMSK = ((TIMSK & TIM0_INTERRUPT_MASK) | TIM0_INTERRUPT_DIS);
			   }
			
			   break;
			
			/*if timer 1*/
			case TIMER_CH1:
			
			    /*check if external clock was chosen*/
			    if(st_Timer_cfg_t->Timer_Mode == COUNTER_FALLING_MODE){
				
				   /*Choose Falling Edge counter*/
				   SET_BIT(TCCR1B,CS11);
				   SET_BIT(TCCR1B,CS10);
			      } 
				
			   else if(st_Timer_cfg_t->Timer_Mode == COUNTER_RISING_MODE){
				
				   /*Choose Rising Edge Counter*/
				   SET_BIT(TCCR0,CS10);
				   SET_BIT(TCCR0,CS12);
				   SET_BIT(TCCR0,CS11);
			   }
			
			   /*check if external clock is clock was chosen*/
			   else if (st_Timer_cfg_t->Timer_Mode == TIMER_MODE){
				
				   /*write the counter mode type here*/
				   gu8_prescalerVal_Timer1 = st_Timer_cfg_t->Timer_Prescaler;
			   }
			
			   /*enable interrupt if it's chosen*/
			   if(st_Timer_cfg_t->Timer_INT_Mode == TIMER_INTERRUPT_MODE){
				
				   /*write 1 at TIMSK TIM1_INTERRUPT BIT*/
				   TIMSK = ((TIMSK & TIM1_INTERRUPT_MASK) | TIM1_INTERRUPT_EN);
			   }
			   /*enable interrupt if it's chosen*/
			   else if(st_Timer_cfg_t->Timer_INT_Mode == TIMER_POLLING_MODE){
				
				   /*write 0 at TIMSK TIM1_INTERRUPT_BIT*/
				   TIMSK = ((TIMSK & TIM1_INTERRUPT_MASK) | TIM1_INTERRUPT_DIS);
			   }
			
			   break;
			
			/*TIMER 2 INITS*/
			case TIMER_CH2:
			
			   /*check if falling edge was chosen*/
			   if(st_Timer_cfg_t->Timer_Mode == COUNTER_FALLING_MODE){
				
					/*return failed because no external at timer2*/
				   return E_NOK;
			   }
			   else if(st_Timer_cfg_t->Timer_Mode == COUNTER_RISING_MODE){
				
				   /*return failed because no external at timer2*/
				   return E_NOK;
			   }

			   /*check if internal clock is clock was chosen*/
			   else if (st_Timer_cfg_t->Timer_Mode == TIMER_MODE){
				
				/*write the prescaler*/
				gu8_prescalerVal_Timer2 = st_Timer_cfg_t->Timer_Prescaler;
			   }
			
			   /*enable interrupt if it's chosen*/
			   if(st_Timer_cfg_t->Timer_INT_Mode == TIMER_INTERRUPT_MODE){
				
				   /*write 1 at TIMSK*/
				   TIMSK = ((TIMSK & TIM2_INTERRUPT_MASK) | TIM2_INTERRUPT_EN);
			   }
			   /*enable interrupt if it's chosen*/
			   else if(st_Timer_cfg_t->Timer_INT_Mode == TIMER_POLLING_MODE){
				
				   /*write 1 at TIMSK*/
				   TIMSK = ((TIMSK & TIM2_INTERRUPT_MASK) | TIM2_INTERRUPT_DIS);
			   }
			
			break;
			   
			default:
			
				/*wrong channel then return failed*/
				error_flag  = E_NOK;
			break;
		   }
		}
		
		   /*return done status*/
		   return error_flag;	
}

	/**
 * Input: 
 * 	u8_Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Vale: The start value of the timer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function strats the needed timer.
 * 							
 */
ERROR_STATUS Timer_Start(uint8_t u8_Timer_CH_NO, uint16_t Timer_Vale){
		
		/*create error flag*/
		ERROR_STATUS error_flag = E_OK;
		
		/*Determine which channel was chosen*/
		switch(u8_Timer_CH_NO){
			
			case TIMER_CH0:
			
			   /*check if the contVal > 255 so don't do it*/
			   if(((uint8_t)Timer_Vale) > TIM0_MAX_CONT){
				
				   /*return Failed*/
				   error_flag = E_NOK;
			   }
			
			   /*write the cont val at TCNT*/
			   else{
				
				/*check the prescaler val*/
				switch(gu8_prescalerVal_Timer0){
						  
						case TIMER_NO_CLOCK:
						  
								/*if No clock then clear psc bits*/
								CLEAR_BIT(TCCR0,CS00);
								CLEAR_BIT(TCCR0,CS01);
								CLEAR_BIT(TCCR0,CS02);
							 
						break;
							 
							 /*if no prescaler*/
					case TIMER_PRESCALER_NO:
						  
								/*for No prescaler set CS00 only and clear other bits*/
								SET_BIT(TCCR0,(CS00));
								CLEAR_BIT(TCCR0,CS02);
								CLEAR_BIT(TCCR0,CS01);
					break;
							 
							 /*for 8 prescaler*/
					case TIMER_PRESCALER_8:
						  
								/*set CS01 Only and clear other bits*/
								SET_BIT(TCCR0,(CS01));
								CLEAR_BIT(TCCR0,(CS00 | CS02));
							 
					break;
						  
						    /*for 64 prescaler*/
					case TIMER_PRESCALER_64:
						  
								/*set CS00, set CS01 and clear CS02*/
								SET_BIT(TCCR0,CS01);
								SET_BIT(TCCR0,CS00);
								CLEAR_BIT(TCCR0,CS02);
							 
					break;
						  
						    /*for265 prescaler*/
					case TIMER_PRESCALER_256:
						  
								/*set CS02 only and clear others*/
								SET_BIT(TCCR0,(CS02));
								CLEAR_BIT(TCCR0,CS01);
								CLEAR_BIT(TCCR0,CS00);
				   break;
						  
						     /*for 1024 prescaler*/
				   case TIMER_PRESCALER_1024:
						  
								/*set CS02 and CS00*/
								SET_BIT(TCCR0,CS02);
								SET_BIT(TCCR0,CS00);
								CLEAR_BIT(TCCR0,CS01);
							 
				   break;
					
					/*if wrong prescale entered then return ENOK*/	  
				   default:
							 error_flag = E_NOK;
				   break;   
					  }
					  
					  /*TIM0_MAX = 255 so for 100 cont TCNT0 = 155*/
					  TCNT0 = TIM0_MAX_CONT-((uint8_t) Timer_Vale);
			   }
			   
			case TIMER_CH1:
			
			   /*check the prescaler val*/
			switch(gu8_prescalerVal_Timer1){
				   
				case TIMER_NO_CLOCK:
				   
						/*if No clock then clear psc bits*/
						CLEAR_BIT(TCCR1B,CS11);
						CLEAR_BIT(TCCR1B,CS10);
						CLEAR_BIT(TCCR1B,CS12);
				break;
				   
						/*if no prescaler*/
				case TIMER_PRESCALER_NO:
				   
						/*for No prescaler set CS10 only and clear other bits*/
						SET_BIT(TCCR1B,(CS10));
						CLEAR_BIT(TCCR1B,CS11);
						CLEAR_BIT(TCCR1B,CS12);
				break;
				   
					/*for 8 prescaler*/
				case TIMER_PRESCALER_8:
				   
						/*set CS11 Only and clear other bits*/
						SET_BIT(TCCR1B,(CS11));
						CLEAR_BIT(TCCR1B,CS10);
						CLEAR_BIT(TCCR1B,CS12);
				break;
				   
					/*for 64 prescaler*/
				case TIMER_PRESCALER_64:
				   
						/*set CS10, set CS11 and clear CS12*/
						SET_BIT(TCCR1B,CS11);
						SET_BIT(TCCR1B,CS10);
						CLEAR_BIT(TCCR1B,CS12);
				   
				break;
				   
					/*for265 prescaler*/
				case TIMER_PRESCALER_256:
				   
						/*set CS12 only and clear others*/
						SET_BIT(TCCR1B,(CS12));
						CLEAR_BIT(TCCR1B,CS10);
						CLEAR_BIT(TCCR1B,CS11);
				   
				break;
				   
				/*for 1024 prescaler*/
			case TIMER_PRESCALER_1024:
				   
						/*set CS12 and CS10*/
						SET_BIT(TCCR1B,CS12);
						SET_BIT(TCCR1B,CS10);
						CLEAR_BIT(TCCR1B,(CS11));
				   
			break;
				   
			/*if wrong prescale entered then return ENOK*/
			default:
						error_flag = E_NOK;
			break;
			}
			
			case TIMER_CH2:
			
			   /*check if the contVal > 255 so don't do it*/
			   if(((uint8_t)Timer_Vale)  > TIM2_MAX_CONT){
				
				   /*return Failed*/
				   error_flag = E_NOK;
			   }
			
			   else{
				   		 /*check the prescaler val*/
				switch(gu8_prescalerVal_Timer2){
					   		 
					case TIMER_NO_CLOCK:
					   		 
					   		 /*if No clock then clear psc bits*/
					   		CLEAR_BIT(TCCR2,CS20);
							CLEAR_BIT(TCCR2,CS22);
							CLEAR_BIT(TCCR2,CS21);
					   		 
					break;
					   		 
					   		 /*if no prescaler*/
				   case TIMER_PRESCALER_NO:
					   		 
					   		 /*for No prescaler set CS10 only and clear other bits*/
					   		 SET_BIT(TCCR1B,(CS20));
					   		 CLEAR_BIT(TCCR1B,CS22);
							 CLEAR_BIT(TCCR1B,CS21);
					   		 
			      break;
					   		 
					/*for 8 prescaler*/
				   case TIMER_PRESCALER_8:
					   		 
					   		 /*set CS21 Only and clear other bits*/
					   		 SET_BIT(TCCR1B,(CS21));
					   		 CLEAR_BIT(TCCR1B,CS22);
							 CLEAR_BIT(TCCR1B,CS20);
					   		 
				   break;
					   		 
					   	/*for 64 prescaler*/
				   case TIMER_PRESCALER_64:
					   		 
					   		 /*set CS20, set CS21 and clear CS22*/
					   		 SET_BIT(TCCR1B,CS20);
							 SET_BIT(TCCR1B,CS21);
					   		 CLEAR_BIT(TCCR1B,CS22);
					   		 
				   break;
					   		 
					   	/*for265 prescaler*/
				   case TIMER_PRESCALER_256:
					   		 
					   		 /*set CS22 only and clear others*/
					   		 SET_BIT(TCCR1B,(CS22));
					   		 CLEAR_BIT(TCCR1B,CS20);
							 CLEAR_BIT(TCCR1B,CS21);	
					   		 
				   break;
					   		 
					   	/*for 1024 prescaler*/
				   case TIMER_PRESCALER_1024:
					   		 
					   		 /*set CS22 and CS20*/
					   		 SET_BIT(TCCR1B,CS22);
							 SET_BIT(TCCR1B,CS20);
					   		 CLEAR_BIT(TCCR1B,(CS21));
					   		 
				   break;
					   		 
			default:
			   
			   /*wrong channel entered then return failed*/
			   error_flag = E_NOK;
			break;
		   }
		}
	}	
		   /*return error state*/
		   return error_flag;
}
/**
 * Input: 
 * 	u8_Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function stops the needed timer.
 * 							
 */
ERROR_STATUS Timer_Stop(uint8_t u8_Timer_CH_NO){
		
		/*create error flag*/
		ERROR_STATUS error_flag = E_OK;
		
		/*check for the entered channel*/
		switch(u8_Timer_CH_NO){
			
		case TIMER_CH0:
			
			/*clear PSC VALUES*/
			TCCR0 = ((TCCR0 & TCCR0_WITHOUT_PSC));
			
		break;
			
		case TIMER_CH1:
			
			/*clear PSC VALUES*/
			TCCR1B = ((TCCR1B & TCCR1B_WITHOUT_PSC));
			
		break;
			
		case TIMER_CH2:
			
			/*Clear PSC VALUES*/
			TCCR2 = ((TCCR2 & TCCR2_WITHOUT_PSC));
			
		break;
		}
		
		/*return error status*/
		return error_flag;
	}


/**
 * Input: 
 * 	u8_Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	u8_Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return the value of the timer.
 * 							
 */
ERROR_STATUS Timer_GetValue(uint8_t u8_Timer_CH_NO, uint16_t* u8_Data){
		
		/*create error flag*/
		ERROR_STATUS error_flag = E_OK;
		
		/*check if the pointer pointing to nothing*/
		if(u8_Data == NULL){
			
			/*failed task*/
			error_flag = E_NOK;
		}
		
		/*determine which channel you working on*/
	switch(u8_Timer_CH_NO){
			
		case TIMER_CH0:
			
			/*write the TCNT val in timerVal*/
			*u8_Data = ((uint16_t)TCNT0);
			
		break;
			
		case TIMER_CH1:
			
			/*write the TCNT VAL IN timerVal*/
			*u8_Data = TCNT1;
			
		break;
			
		case TIMER_CH2:
			
			/*write the TCNT VAL in Timer VAL*/
			*u8_Data = ((uint16_t)TCNT2);
			
		break;
			
		default:
		
			/*wrong channel entered the return failed*/
			error_flag = E_NOK;
			
		break;
		}
		
		/*return error status*/
		return error_flag;
}

/**
 * Input: 
 * 	u8_Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	u8_Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return if the flag of the timer is raised or not.
 * 							
 */
ERROR_STATUS Timer_GetStatus(uint8_t u8_Timer_CH_NO, uint8_t* u8_Data){
	
		/*create error flag*/
		ERROR_STATUS error_flag = E_OK;
		
		/*check if pointer pointing to nothing*/
		if(u8_Data == NULL){
			
			/*return failed task*/
			error_flag = E_NOK;
		}
		
		switch(u8_Timer_CH_NO){
			
		/*if channel 0 then check it's ovf flag*/
		case TIMER_CH0:
			
			if(BIT_IS_SIT(TIFR,TIMER0_OVERFLOW)){
				
				/*write 1 to ovf status*/
				*u8_Data = OVF_FLAG_SET;
				
				/*clear the flag*/
				TIFR |= TIMER0_OVERFLOW;
			}
			
			else{
				/*no ovf then write zero*/
				*u8_Data = OVF_FLAG_CLEAR;
			}
			
		break;
			
		/*if channel 1 then check it's ovf flag*/
		case TIMER_CH1:
			
			if(BIT_IS_SIT(TIFR,TIMER1_OVERFLOW)){
				
				/*write 1 to ovf status*/
				*u8_Data = OVF_FLAG_SET;
				
				/*clear the flag*/
				TIFR |= TIMER1_OVERFLOW;
			}
			
			else{
				/*no ovf then write zero*/
				*u8_Data = OVF_FLAG_CLEAR;
			}
			
		break;
			
		/*if channel 1 then check it's ovf flag*/
		case TIMER_CH2:
			
			if(BIT_IS_SIT(TIFR,TIMER2_OVERFLOW)){
				
				/*write 1 to ovf status*/
				*u8_Data = OVF_FLAG_SET;
				
				/*clear the flag*/
				TIFR |= TIMER2_OVERFLOW;
			}
			
			else{
				/*no ovf then write zero*/
				*u8_Data = OVF_FLAG_CLEAR;
			}
			
		break;
			
		default:
			   
			   /*wrong channel entered then return failed*/
			   error_flag = E_NOK;
			   
		break;
		}
		
		/*return error state*/
		return error_flag;
}

/**
*Input : Number of channel " 0 / 1/ 2", u16_Data: output pointer to value
*return : E_NOK if an error occured, E_OK: if it worked fine
*/


ERROR_STATUS Timer_SetValue(uint8_t u8_Timer_CH_NO,uint16_t u16_Data){
	
	/*create error flag*/
	ERROR_STATUS error_flag = E_OK;
	
	/*switch for the channel you want to read it's Val "0/1/2" only*/
	switch(u8_Timer_CH_NO){
		
		case TIMER_CH0:
		
		   /*read TCNT Value of TMR0 which is 8bits only wide*/
		   TCNT0 = (uint8_t)u16_Data ;
		   
		   break;
		   
		case TIMER_CH1:
		
		   /*read TCNT Value of TMR1 which is 16 bit wide*/
		   (TCNT1) =u16_Data;
		   
		   break;
		   
		case TIMER_CH2:
		
		   /*read TCNT Value of TMR2 Which is 16bit wide*/
		    (TCNT2) =u16_Data;
		   
		   break;
		   
		default:
		   
		   /*not correct channel entered so return not ok*/
		   error_flag = E_NOK;
		   
		break;
	}

	/*return error flag*/
	return error_flag;
}	

ISR(TIMER0_OVF_irq)
{
	ptr_callback();
}