/*
 * can.h
 *
 *  Created on: Jun 8, 2020
 *      Author: ahmed
 */

#ifndef MCAL_CAN_CAN_H_
#define MCAL_CAN_CAN_H_

/*Includes*/
#include <SERVICES/Error_table.h>
#include <SERVICES/std_types.h>

/*Macros*/
#define STANDARD_ID		(0U)
#define EXTENDED_ID		(1U)

#define DATA_FRAME		(0U)
#define REMOTE_FRAME	(1U)

#define CAN_MODULE_1 	(0u)
#define CAN_MODULE_2	(1u)

#define RX_MAX_ARRAY_SIZE	(6U)
/*Array of struct for received data*/
typedef struct
{
	uint8_t msg[8];
	uint8_t msg_size;
	uint32_t ID;
	uint8_t ID_Type:1;
}str_RxMsg_t;

extern str_RxMsg_t arr_msgs[RX_MAX_ARRAY_SIZE];

/*****************PROTOTYPES*******************/

/*Can init function
 * this is a linked config file so search for the canlcfg.c file to configure what to write in can
 * input : None but externed struct used
 * output : Error if found
 */
ERROR_STATUS CanInit(void);

/*Send data through can function
 * Description : a function used to configure a full frame for can tx to be sent
 * Input : RTR  , IDE , Identifier and data to be sent
 * Output : Error if found
 */
ERROR_STATUS Can_Tx(uint8_t IDE, uint8_t RTR , uint32_t ID, uint8_t *Data , uint8_t Can_Num , uint8_t BytesNo);

/*Reception
 * Description : Function To receive data from Can receiver
 * note : please configure CAn pins before using
 * Input : Data pointer , Data length
 * Output : Error if found
 */
ERROR_STATUS Can_Rx(uint8_t Can_ID);

#endif /* MCAL_CAN_CAN_H_ */
