/*
 * Steering.c
 *
 * Created: 18-Feb-20 10:49:09 PM
 *  Author: ahmed
 */ 
#include "Steering.h"
#include "/carApp/carApp/carApp/ECUAL/motor.h"

/************************************************************************/
/*					APIS                                                */
/************************************************************************/

/**
 * Input: 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */
ERROR_STATUS Steering_Init(void){
	
	/*create error_flag*/
	ERROR_STATUS error_flag = E_OK;
	
	/*init both motors*/
	error_flag |= Motor_Init(MOTOR_1);
	error_flag |= Motor_Init(MOTOR_2);
	
	return error_flag;
}

/**
 * Input: 
 * 	carDirection: The direction of the car from the predefined states of the module.
 * 	speed: The speed of the car from 0% to 100%.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This funtion controls the command and the speed of the car.
 * 							
 */
ERROR_STATUS Steering_SteerCar(uint8_t Steering_CarCmd, uint8_t speed){
	
	ERROR_STATUS error_flag = E_OK;
	
	/*choose which direction to use*/
	switch(Steering_CarCmd){
		
		/*for forward direction set both motor forward*/
		case CAR_FORWARD:
			
			/*set direction as forward*/
			error_flag |= Motor_Direction(MOTOR_1,MOTOR_FORWARD);
			error_flag |= Motor_Direction(MOTOR_2,MOTOR_FORWARD);
			
		break;
		
		case CAR_BACKWARD:
			
			/*set both directions as backward*/
			error_flag |= Motor_Direction(MOTOR_1,MOTOR_BACKWARD);
			error_flag |= Motor_Direction(MOTOR_2,MOTOR_BACKWARD);
			
		break;
		
		case CAR_LEFT:
		
			/*set 1 direction as forward and the other one as backward*/
			error_flag |= Motor_Direction(MOTOR_1,MOTOR_FORWARD);
			error_flag |= Motor_Direction(MOTOR_2,MOTOR_BACKWARD);
		break;
		
		case CAR_RIGHT:
			
			/*set 1 direction as backward and 2 as forward*/
			error_flag |= Motor_Direction(MOTOR_1,MOTOR_BACKWARD);
			error_flag |= Motor_Direction(MOTOR_2,MOTOR_FORWARD);
		break;
		
		default:
			
			/*wrong entered direction then not okay*/
			error_flag = E_NOK;
		break;
		
	}
	
	/*enter both speeds of timer1/2*/
	error_flag |= Motor_Start(MOTOR_1,speed);
	error_flag |= Motor_Start(MOTOR_2,speed);
	
	/*return error status*/
	return error_flag;
}

