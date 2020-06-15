/*
 * CANlcfg.h
 *
 *  Created on: Jun 6, 2020
 *      Author: ahmed
 */

#ifndef MCAL_CAN_CANLCFG_H_
#define MCAL_CAN_CANLCFG_H_

/*includes*/
#include <SERVICES/std_types.h>

/*CONFIG Macros*/

/*MCR Macros*/

/*array macros*/
#define NUM_OF_CONFIGS (1)
#define NUM_OF_FILTERS (1)

/*Can IDS*/
#define CAN_1					(0u)
#define CAN_2					(1u)

#define DEBUG_ON  				(0)
#define FREEZE_ON 				(1)
#define FORCE_RESET 			(1)
#define NORMAL_MODE 			(0)
#define TIME_TRIGGER_OFF 		(0)
#define TIME_TRIGGER_ON  		(1)
#define AUTO_BUS_OFF_SW 		(0)
#define AUTO_BUS_OFF_HW 		(1)
#define MANUAL_WAKE_UP 			(0)
#define AUTO_WAKE_UP 			(1)
#define AUTO_TRANSMIT_ON  		(0)
#define AUTO_TRANSMIT_OFF 		(1)
#define TX_FIFO_PRIORITY_ID 	(0)
#define TX_FIFO_PRIORITY_ORDER 	(1)
#define SLEEP_ON				(1)
#define SLEEP_OFF				(0)
#define INIT_REQUEST_SET		(1)
#define INIT_REQUEST_CLEAR		(0)

/*IRQ Macros*/
#define IRQ_OFF 				(0)
#define IRQ_ON					(1)

/*BIT TIME MACROS*/
#define SILENT_MODE_ON			(1)
#define SILENT_MODE_OFF			(0)
#define LOOPBACK_MODE_ON		(1)
#define LOOPBACK_MODE_OFF		(0)

/*filter macros*/
#define FILTER_INIT_ON 			(1)
#define FILTER_INIT_OFF			(0)

#define FILTER_MASK_MODE		(0)
#define FILTER_IDLIST_MODE		(1)

#define SINGLE_32BIT_CONF		(1)
#define DUAL_16BIT_CONF			(0)

#define FILTER_FIFO_1			(1)
#define FILTER_FIFO_0			(0)

#define FILTER_ACTIVE			(1)
#define FILTER_NOT_ACTIVE		(0)

#define FIFO_0					(0u)
#define FIFO_1					(1u)

typedef struct
{
	uint8_t RTR:1;
	uint8_t IDE:1;
	uint32_t ID;
	uint32_t Mask;

}str_FILTERS_t;

typedef struct
{
	uint8_t CAN_ID:1;
	/*MCR*/
	uint8_t DEBUG_STATE;
	uint8_t TIME_TRIGGER_MODE;
	uint8_t BUS_OFF;
	uint8_t AUTO_WAKEUP;
	uint8_t AUTO_RETRANSMIT;
	uint8_t FIFO_LOCK;
	uint8_t TRANSMIT_PRIORITY;
	uint8_t SLEEP_MODE;
	uint8_t INIT_REQ;

	/*INTERRUPT*/
	uint8_t SLEEP_IRQ;
	uint8_t WAKE_IRQ;
	uint8_t ERROR_IRQ;
	uint8_t LAST_ERROR_IRQ;
	uint8_t BOFF_IRQ;
	uint8_t ERR_PASSIVE_IRQ;
	uint8_t ERR_WARN_IRQ;
	/*FIFO 1 IRQ*/
	uint8_t OVERRUN_IRQ_1;
	uint8_t FULL_FIFI_IRQ_1;
	uint8_t FIFO_PEND_IRQ_1;
	/*FIFO 0 IRQ*/
	uint8_t OVERRUN_IRQ_0;
	uint8_t FULL_FIFI_IRQ_0;
	uint8_t FIFO_PEND_IRQ_0;

	uint8_t TX_EMPTY_IRQ;

	/*BIT TIME*/
	uint8_t SILENT_MODE;
	uint8_t LOOP_BACK_MODE;
	uint8_t RESYNCH_JUMP;
	uint8_t TIME_SEG_1;
	uint8_t TIME_SEG_2;
	uint16_t BAUD_RATE;

	/*Filter*/
	uint8_t filter_init:1;
	uint8_t CAN2_START_BANK;

	uint8_t RTR:1;
	uint8_t IDE:1;

	uint8_t Dual_Mode:1;
	uint8_t FILTER_MODE:1;
	uint8_t FIFO_USED;

	str_FILTERS_t filter_conf[28];

}str_CANlcfg_t;

/*Extern array here*/
extern str_CANlcfg_t arrCan[NUM_OF_CONFIGS];

#endif /* MCAL_CAN_CANLCFG_H_ */
