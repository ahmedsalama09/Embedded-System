/*
 * Timer.h
 *
 *  Created on: Dec 21, 2019
 *      Author: Zayed & Saad
 */

#ifndef TIMER_H_
#define TIMER_H_

/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/
#include "/Sprint projects/SOS&CarApp/Car by sos/SERVICE/std_types.h"


/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/

#define TIMER0_OVERFLOW   0
#define TIMER1_OVERFLOW   2
#define TIMER2_OVERFLOW   6


#define TIM0_INTERRUPT_MASK   (0xFE)
#define TIM0_INTERRUPT_EN   (0x01)
#define TIM0_INTERRUPT_DIS   (0x00)

#define TIM1_INTERRUPT_MASK   (0xC3)
#define TIM1_INTERRUPT_EN   (0x04)
#define TIM1_INTERRUPT_DIS   (0x00)

#define TIM2_INTERRUPT_MASK   (0x3F)
#define TIM2_INTERRUPT_EN   (0x40)
#define TIM2_INTERRUPT_DIS   (0x00)

#define TIM0_MAX_CONT   (255)
#define TCCR0_WITHOUT_PSC   (0xF8)

#define TIM1_MAX_CONT   (65535)
#define TCCR1B_WITHOUT_PSC   (0xF8)

#define TIM2_MAX_CONT   (255)
#define TCCR2_WITHOUT_PSC   (0xF8)
/************************************************************************/
/*					CHANNELS DEFINES	                                */
/************************************************************************/
#define		TIMER_CH0					0
#define		TIMER_CH1					1
#define		TIMER_CH2					2

/************************************************************************/
/*					TIMER MODES DEFINES                                 */
/************************************************************************/
#define		TIMER_MODE					0
#define		COUNTER_RISING_MODE					1
#define		COUNTER_FALLING_MODE				2

/************************************************************************/
/*				TIMER INTERRUPT OR POLLING MODE                         */
/************************************************************************/
#define		TIMER_POLLING_MODE				0
#define		TIMER_INTERRUPT_MODE			1

/************************************************************************/
/*				TIMER PRESCALLER                                        */
/************************************************************************/
#define		TIMER_NO_CLOCK					0
#define		TIMER_PRESCALER_NO				1
#define		TIMER_PRESCALER_8				2
#define		TIMER_PRESCALER_32				3
#define		TIMER_PRESCALER_64				4
#define		TIMER_PRESCALER_128				5
#define		TIMER_PRESCALER_256				6
#define		TIMER_PRESCALER_1024			7

/************************************************************************/
/*			EXTERNAL TIMER MODE                                         */
/************************************************************************/
#define COUNTER_FALLING_EDGE   (6)
#define COUNTER_RISING_EDGE   (7)

/************************************************************************/
/*			  Structures Definitions		                            */
/************************************************************************/

typedef struct Timer_cfg_s
{
	uint8_t Timer_CH_NO;
	uint8_t Timer_Mode;
	uint8_t Timer_INT_Mode;
	uint8_t Timer_Prescaler;
	volatile void (*Timer_Cbk_ptr)(void);
}Timer_cfg_s;



/************************************************************************/
/*		         TIMER FUNCTIONS' PROTOTYPES		        */
/************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the timer. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */
extern ERROR_STATUS Timer_Init(Timer_cfg_s* st_Timer_cfg_t);

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Count: The start value of the timer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function strats the needed timer.
 * 							
 */
extern ERROR_STATUS Timer_Start(uint8_t Timer_Ch, uint16_t Timer_Vale);

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function stops the needed timer.
 * 							
 */
extern ERROR_STATUS Timer_Stop(uint8_t Timer_Ch);

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return if the flag of the timer is raised or not.
 * 							
 */
extern ERROR_STATUS Timer_GetStatus(uint8_t Timer_Ch, uint8_t* Timer_Vale);

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return the value of the timer.
 * 							
 */
extern ERROR_STATUS Timer_GetValue(uint8_t Timer_Ch, uint16_t* Timer_Vale);

/**
*Input : Number of channel " 0 / 1/ 2", u16_Data: output pointer to value
*return : E_NOK if an error occured, E_OK: if it worked fine
*/


extern ERROR_STATUS Timer_SetValue(uint8_t u8_Timer_CH_NO,uint16_t u16_Data);







#endif /* TIMER_H_ */