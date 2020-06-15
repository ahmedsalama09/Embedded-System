/*Author: Ahmed Salama
 * File to let the user configure can from here
 */

/*Includes*/
#include "CANlcfg.h"

/*ALL USER CONFIGURE NUM MACROS HERE*/

/*Please use this link to define bit time segments: http://www.bittiming.can-wiki.info/*/

#define RESYNCH_VAL (0u)
#define TIME_SEG_1_VAL (11u)
#define TIME_SEG_2_VAL (2u)
#define BAUD_RATE_VAL  (1000u)
#define START_BANK_VAL (1)

str_CANlcfg_t arrCan[NUM_OF_CONFIGS] = {{.CAN_ID = CAN_1,
										 .DEBUG_STATE = DEBUG_ON,
										 .TIME_TRIGGER_MODE = TIME_TRIGGER_ON,
										 .BUS_OFF = AUTO_BUS_OFF_HW,
										 .AUTO_WAKEUP = AUTO_WAKE_UP,
										 .AUTO_RETRANSMIT = AUTO_TRANSMIT_ON,
										 .SLEEP_MODE = SLEEP_OFF,
										 .INIT_REQ = INIT_REQUEST_SET,
										 .SLEEP_IRQ = IRQ_OFF,
										 .WAKE_IRQ = IRQ_OFF,
										 .ERROR_IRQ = IRQ_OFF,
										 .LAST_ERROR_IRQ = IRQ_OFF,
										 .BOFF_IRQ = IRQ_OFF,
										 .ERR_PASSIVE_IRQ = IRQ_OFF,
										 .ERROR_IRQ = IRQ_OFF,
										 .ERR_WARN_IRQ = IRQ_OFF,
										 .OVERRUN_IRQ_1 = IRQ_OFF,
										 .FULL_FIFI_IRQ_1 = IRQ_OFF,
										 .FIFO_PEND_IRQ_1 = IRQ_OFF,
										 .OVERRUN_IRQ_0 = IRQ_OFF,
										 .FULL_FIFI_IRQ_0 = IRQ_OFF,
										 .FIFO_PEND_IRQ_0 = IRQ_OFF,
										 .TX_EMPTY_IRQ = IRQ_OFF,
										 .SILENT_MODE = SILENT_MODE_ON,
										 .LOOP_BACK_MODE = LOOPBACK_MODE_ON,
										 .RESYNCH_JUMP = RESYNCH_VAL,
										 .TIME_SEG_1 = TIME_SEG_1_VAL,
										 .TIME_SEG_2 = TIME_SEG_2_VAL,
										 .BAUD_RATE = BAUD_RATE_VAL,
										 .filter_init = FILTER_INIT_ON,
										 .CAN2_START_BANK = START_BANK_VAL}};
