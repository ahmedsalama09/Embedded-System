/*
 * car_sm.c
 *
 * Created: 19-Feb-20 12:42:11 AM
 *  Author: ahmed
 */

/************************************************************************/
/*							INCLUDES                                    */
/************************************************************************/ 
#include "car_sm.h"
#include "Steering.h"
#include "/carApp/carApp/carApp/ECUAL/Us.h"
#include "/carApp/carApp/carApp/SERVICES/software_delay.h"

/************************************************************************/
/*					DEFINES                                             */
/************************************************************************/
#define CAR_SPEED	  (30)
#define DISTANCE_25   (25)
#define DISTANCE_45   (45)
#define DELAY_MS_125   (125)
#define FLAG_SET   (1)
#define FLAG_CLEAR   (0)

#define FORWARD		(0)
#define BACKWARD	(1)
#define LEFT		(2)
#define INIT_DIR	(3)

#define MSECOND_150   (150)
#define MSECOND_125	  (125)
/*************************************************************/
/*                     APIS					                */
/*************************************************************/

/*
 * Fun----------: ERROR_STATUS Car_SM_Init(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: Initiate the car state machine with state"forward at speed 80%",
 * -------------- And initiate steering and ultrasonic functions
*/
ERROR_STATUS Car_SM_Init(void){
	
	ERROR_STATUS error_flag = E_OK;
	
	/*init stearing and ultrasonic*/
	error_flag |=  Steering_Init();
	error_flag |= Us_Init();
	
	return error_flag;
}

/*
 * Fun----------: ERROR_STATUS Car_SM_Update(void);
 * Input--------: Nothing
 * Output-------: Nothing
 * Return-------: ERROR_STATUES
 * -------------- #E_Ok	->function operation is okay, no error
 * -------------- #E_Nok->function operation faild, error exists
 * Description--: changes car state according to the ultrasonic input
 * -------------- And represents the motors output for each state
*/
ERROR_STATUS Car_SM_Update(void){
	
	/*create error flag*/
	ERROR_STATUS error_flag = E_OK;
	
	/*flag to indicate at another backward condition*/
	uint8_t u8_BackFlag=FLAG_CLEAR;
	
	/*direction flag*/
	uint8_t u8_DirectionFlag = INIT_DIR;
	
	/*distance value*/
	uint16_t u16_distance=LOW;
	
	
	/*send trigger of US*/
	Us_Trigger();
	/*read distance from ultrasonic*/
	error_flag |= Us_GetDistance(&u16_distance);
	
	/*for distance more than 25 keep it forward*/
	if(u16_distance <= DISTANCE_25){
		
		/*direction must be back*/
		u8_DirectionFlag = BACKWARD;
	}
	/*for that range go left*/
	else if((u16_distance >= DISTANCE_25 && u16_distance <= DISTANCE_45) || u8_BackFlag ==FLAG_SET){
		
		/*go left*/
		u8_DirectionFlag = LEFT;
	}
	else if(u16_distance > DISTANCE_45){
		
		/*SET state as forward*/
		u8_DirectionFlag = FORWARD;
	}

	/*make 125 delay between each trigger*/	
	error_flag |= software_delay(MSECOND_125);
	
	switch(u8_DirectionFlag){
		
		case LEFT:
			
			/*set stearing direction as left */
			error_flag |= Steering_SteerCar(CAR_LEFT,CAR_SPEED);
			error_flag |= software_delay(MSECOND_150);
			u8_BackFlag = FLAG_CLEAR;
			
		break;
		
		case FORWARD:
			
			/*Set forward direction*/
			error_flag |= Steering_SteerCar(CAR_FORWARD,CAR_SPEED);
			
		break;
		
		case BACKWARD:
			
			/*set Backward Direction*/
			error_flag |= Steering_SteerCar(CAR_BACKWARD,CAR_SPEED);
			
			/*set BACKWARD Flag*/
			u8_BackFlag = FLAG_SET;
		break;
	}
	
	/*return error status*/
	return error_flag;
}