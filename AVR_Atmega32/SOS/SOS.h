/*
 * TMU.h
 *
 * Created: 24-Feb-20 11:32:41 AM
 *  Author: ahmed
 */ 


#ifndef TMU_H_
#define TMU_H_

/************************************************************************/
/*							INCLUDES                                    */
/************************************************************************/
#include "/tmu/tmuapp/tmuapp/common_macros.h"
#include "/tmu/tmuapp/tmuapp/std_types.h"
#include "SOS_Config.h"

/************************************************************************/
/*							DEFINES                                     */
/************************************************************************/
#define SOS_ONESHOOT	(2)
#define SOS_PERIODIC	(1)

/*timer channels*/
#define SOS_TMR_CH0		(0)
#define SOS_TMR_CH1		(1)
#define SOS_TMR_CH2		(2)

/************************************************************************/
/*					DATA TYPES                                          */
/************************************************************************/
typedef void (*callback) ();

/*TMU_struct 
**elements periodic_state to state if function is repetitive or not
*callback address
*number of counts
*/
typedef struct{
	uint8_t u8_periodic_state;
	uint8_t u8_id;
	uint32_t u32_counts;
	uint8_t u8_TaskState;
	uint8_t u8_TaskPriority;
	callback task_ptr;
	}st_SOS_TaskType_t;

typedef struct{
	uint8_t u8_timer_chnl;
	}st_SOS_init_t;

/** Init TMU function
**Description : function takes pointer to struct of TMU init
*				struct has one variable which is the timer at which the TMU will operate
	
** @parm : pointer to TMU - Init type struct

*return "error_state* : -1 : for NULL POINTER
						-2 : for multiple initialization
						
*/
ERROR_STATUS SOS_Init(st_SOS_init_t *ConfigPtr);

/**TMU_START
*Description : function to start the chosen task by inserting it in the schedular
** INPUTS  : pointer to struct of TMU_Task @ which 1- task id 2-task counts 3- task state
4-task callback pointer
*Return : Error state
*/
ERROR_STATUS SOS_Start_Timer(st_SOS_TaskType_t *Tmu_start);

/** TMU_DISPATCH
*Description : Function that starts the timer and trigger the scheduled tasks
*Input : none
*Output : ERROR STATUS: NO_ERROR , ERROR_NO_TASK if no task is inserted*/
ERROR_STATUS  SOS_Dispatch(void);

/* Description : function idicates which Task is nolonger operating
** Inputs : Task_ID
* Output : ERROR_STATUS 1-NO_ERROR 2_MODULE NOT STARTED
*/
ERROR_STATUS SOS_Stop_Timer(uint8_t Task_ID);

/** TMU_deinit module
*Description : return everything to the starting point and unitialize all configs
** Output : nothing
*/
void SOS_DeInit(void);
#endif /* TMU_H_ */