/*
 * bcm.h
 *
 * Created: 2020-03-01 9:49:57 PM
 *  Author: EbrahimOseif
 */ 


#ifndef BCM_H_
#define BCM_H_


/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/

#include "/BCM/Basic_Comm_Module/Basic_Comm_Module/std_types.h"
#include "bcm_cfg.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/

/* BCM_ID */

#define BCM_ID                                  (0x7F)

/* BCM_Dispatcher states */
#define BCM_DISPATCHER_IDLE                     (0)

/* TX states */
#define BCM_DISPATCHER_TRANSMIT_BYTE            (1)
#define BCM_DISPATCHER_TRANSMIT_BYTE_COMPLETE   (2)

/* RX states */
#define BCM_DISPATCHER_RECEIVE_BYTE             (3)
#define BCM_DISPATCHER_RECEIVE_BYTE_COMPLETE    (4)


/* BCM_TxDispatcher Transmit states */
#define BCM_TX_DISPATCHER_ID_TX                 (0)
#define BCM_TX_DISPATCHER_SIZEL_TX              (1)
#define BCM_TX_DISPATCHER_SIZEH_TX              (2)
#define BCM_TX_DISPATCHER_DATA_TX               (3)

/* BCM TX buffer lock states */
#define BCM_TX_BUFFER_UNLOCKED                  (0)
#define BCM_TX_BUFFER_LOCKED                    (1)

/* BCM send state */
#define BCM_SEND_NOT_TRIGGERED                  (0)
#define BCM_SEND_TRIGGERED                      (1)

/* BCM TX ISR Flag */
#define BCM_TXC_FLAG_NOT_TRIGGERED              (0)
#define BCM_TXC_FLAG_TRIGGERED                  (1)

/************************************************************************/
/*				 PRIMITIVE TYPES			        */
/************************************************************************/


/* define buffer pointer as pointer to 8-bit integer */
typedef uint8_t* ptrBuffer;

/* Transmit complete User callback function typedef */
typedef void (*gptr_BCM_TxC_CBK_Func_t) (void);

/* Receive complete User callback function typedef */
typedef void (*gptr_BCM_ReceiveComplete_User_CBK_t) (void);





/************************************************************************/
/*		               FUNCTIONS' PROTOTYPES		                    */
/************************************************************************/

/**
* @brief: BCM Initialization
* @param: gptr_BCM_TxComplete_CBK: pointer to user TxComplete callback
* @return: ERROR_STATUS status code with error code if one occurred
*/
ERROR_STATUS BCM_Init(gptr_BCM_TxC_CBK_Func_t gptr_BCM_TxComplete_CBK);

/**
* @brief: takes pointer to the TX buffer and
* 	  its size, checks if size is <= MAX_BUFFER_SIZE,
* 	  and passes them to the dispatcher.
* @param: ptr_Buffer, u16_Size
* @return: ERROR_STATUS status code with error code if one occurred
*/
ERROR_STATUS BCM_Send(ptrBuffer ptr_Buffer, uint16_t u16_Size);

/**
* @brief: takes pointer to the RX buffer
* 	  and passes it to the dispatcher.
* @param: ptr_Buffer
* @return: ERROR_STATUS status code with error code if one occurred
*/
ERROR_STATUS BCM_SetupReceive(ptrBuffer ptr_Buffer);

/**
* @brief: Handles data reception.
* @param: void
* @return: ERROR_STATUS status code with error code if one occurred
*/
ERROR_STATUS BCM_RxDispatcher(void);

/**
* @brief: Handles data transmission.
* @param: void
* @return: ERROR_STATUS status code with error code if one occurred
*/
ERROR_STATUS BCM_TxDispatcher(void);

/**
* @brief: Indicates user has finished processing data
* 	  in the Rx_Buffer.
* @param: void
* @return: ERROR_STATUS status code with error code if one occurred
*/


#endif /* BCM_H_ */