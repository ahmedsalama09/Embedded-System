/*
 * bcm.c
 *
 * Created: 2020-03-01 9:50:15 PM
 *  Author: EbrahimOseif
 */ 


/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/

//#include "/BCM/Basic_Comm_Module/Basic_Comm_Module/Error.h"
#include "/BCM/Basic_Comm_Module/Basic_Comm_Module/MCAL/uart.h"
#include "/BCM/Basic_Comm_Module/Basic_Comm_Module/SERVICE/bcm.h"
#include "/BCM/Basic_Comm_Module/Basic_Comm_Module/std_types.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/


/*- LOCAL Data types --------------------------------------*/



static void BCM_OneByte_TxComplete_CBK(void);


static void BCM_OneByte_RxComplete_CBK(void);


/************************************************************************/
/*				 GLOBAL STATIC VARIABLES            			        */
/************************************************************************/

extern init_uart st_uart_init;




static uint8_t gu8_BCM_InitState = NOT_INITIALIZED;

static ptrBuffer gptr_TxBuffer = NULL;
static uint16_t gu16_TxSize = 0;

static uint8_t gu8_BCM_TxDispatcher_State			= BCM_DISPATCHER_IDLE;
static uint8_t gu8_BCM_TxDispatcher_Trasmit_State   = BCM_TX_DISPATCHER_ID_TX;
static uint8_t gu8_BCM_TxBuffer_LockState			= BCM_TX_BUFFER_UNLOCKED;
static uint8_t gu8_BCM_SendFlag						= BCM_SEND_NOT_TRIGGERED;

static uint8_t gu8_BCM_TxBuffer_Index;
static uint8_t gu8_BCM_TxChecksum;

static gptr_BCM_TxC_CBK_Func_t gptr_BCM_TxC_CBK_Func = NULL;
static volatile uint8_t gu8_BCM_TxC_Flag = BCM_TXC_FLAG_NOT_TRIGGERED;


/************************************************************************/
/*				 FUNCTIONS IMPLEMENTATION            			        */
/************************************************************************/



ERROR_STATUS BCM_Init(gptr_BCM_TxC_CBK_Func_t gptr_BCM_TxC_CBK)
{
	ERROR_STATUS ERR_Status = BCM_ERROR_BASE + NO_ERRORS;

	
	/* Configure UART */
	st_uart_init.UART_Cbk_Function = gptr_BCM_TxC_CBK;

	/* TODO: Edit UART error handling */
	/* Initialize UART */
	if( uart_init(&st_uart_init) == E_NOK)
	{
		ERR_Status = BCM_ERROR_BASE + ERROR_NOT_INITIALIZED;
	}
	else
	{
		/* Reset global variable to the init state */
		
		
		gptr_TxBuffer = NULL;
		gu16_TxSize = 0;
		gu8_BCM_TxBuffer_Index = 0;


		gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_IDLE;
		gu8_BCM_TxChecksum = 0;
		gu8_BCM_TxC_Flag = BCM_TXC_FLAG_NOT_TRIGGERED;

		if(NULL == gptr_BCM_TxC_CBK)
		{
			ERR_Status = BCM_ERROR_BASE + ERROR_NULL_POINTER;
		}
		else
		{
			gptr_BCM_TxC_CBK_Func = gptr_BCM_TxC_CBK;
		}

		/* Indicate BCM is initialized */
		gu8_BCM_InitState = INITIALIZED;
	}
	return ERR_Status;
}

/**
* @brief: takes pointer to the TX buffer and
* 	  its size, checks if size is <= MAX_BUFFER_SIZE,
* 	  and passes them to the dispatcher.
* @param: ptr_Buffer, u16_Size
* @return: sint8 status code with error code if one occurred
*//*
ERROR_STATUS BCM_Send(ptrBuffer ptr_Buffer, uint16_t u16_Size)
{
	ERROR_STATUS ERR_Status = BCM_ERROR_BASE + NO_ERRORS;

	/ * Check if ptr_Buffer points to NULL * /
	if(NULL == ptr_Buffer)
	{
		/ * Error NULL pointer * /
		ERR_Status = BCM_ERROR_BASE + ERROR_NULL_POINTER;
	}
	else
	{
		gptr_TxBuffer = ptr_Buffer;
		gu16_TxSize   = u16_Size;
		gu8_BCM_SendFlag = BCM_SEND_TRIGGERED;
	}
	return ERR_Status;
}

/ **
* @brief: takes pointer to the RX buffer
* 	  and passes it to the dispatcher.
* @param: ptr_Buffer
* @return: sint8 status code with error code if one occurred
* /
ERROR_STATUS BCM_SetupReceive(ptrBuffer ptr_Buffer)
{

}

/ **
* @brief: Handles data reception.
* @param: void
* @return: sint8 status code with error code if one occurred
* /
ERROR_STATUS BCM_RxDispatcher(void)
{

}

/ **
* @brief: Handles data transmission.
* @param: void
* @return: sint8 status code with error code if one occurred
* /
ERROR_STATUS BCM_TxDispatcher(void)
{
	ERROR_STATUS ERR_Status = BCM_ERROR_BASE + NO_ERRORS;


	switch(gu8_BCM_TxDispatcher_State)
	{
		case (BCM_DISPATCHER_IDLE):
		
		             if((gu8_BCM_SendFlag == BCM_SEND_TRIGGERED) 
		             		&& (gu8_BCM_TxBuffer_LockState == BCM_TX_BUFFER_UNLOCKED))
		             {
		             	/ * Lock the TX buffer * /
		             	gu8_BCM_TxBuffer_LockState = BCM_TX_BUFFER_LOCKED;
		             
		             	uart_tx_send(BCM_ID);
		             
		             	gu8_BCM_TxDispatcher_Trasmit_State = BCM_TX_DISPATCHER_SIZEL_TX;
		             	gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_TRANSMIT_BYTE;
		             }
		             else
		             {
		             	/ * Do nothing * /
		             }
		break;
		
		case (BCM_DISPATCHER_TRANSMIT_BYTE):
		
		              if(gu8_BCM_TxC_Flag == BCM_TXC_FLAG_TRIGGERED)
		              {
		              	/ * Reset BCM_TxC flag * /
		              	gu8_BCM_TxC_Flag = BCM_TXC_FLAG_NOT_TRIGGERED;
		              
		              	if(gu8_BCM_TxDispatcher_Trasmit_State == BCM_TX_DISPATCHER_SIZEL_TX)
		              	{
		              		uart_tx_send((uint8_t)gu16_TxSize);
		              
		              		gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_TRANSMIT_BYTE_COMPLETE;
		              	}
		              	else if(gu8_BCM_TxDispatcher_Trasmit_State == BCM_TX_DISPATCHER_SIZEH_TX)
		              	{
		              		uart_tx_send((uint8_t)(gu16_TxSize >> 8));
		              
		              		gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_TRANSMIT_BYTE_COMPLETE;
		              	}
		              	
		              	
		              	else if(gu8_BCM_TxDispatcher_Trasmit_State == BCM_TX_DISPATCHER_DATA_TX)
		              	{
		              		/ * Loop through TX buffer and transmit each byte * /
		              		if(gu8_BCM_TxBuffer_Index < gu16_TxSize)
		              		{
		              			/ * Send the byte which index is pointing at * /
		              			uart_tx_send(*gptr_TxBuffer);
		              			gu8_BCM_TxChecksum += *gptr_TxBuffer;
		              
		              			gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_TRANSMIT_BYTE_COMPLETE;
		              		}
		              		else
		              		{
		              			/ * Send checksum * /
		              			uart_tx_send(gu8_BCM_TxChecksum);
		              
		              			gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_TRANSMIT_BYTE_COMPLETE;
		              		}
		              	}
		              }
		              else
		              {
		              	/ * Do nothing * /
		              }
		
		   break;
		
		case (BCM_DISPATCHER_TRANSMIT_BYTE_COMPLETE):
				
				switch (gu8_BCM_TxDispatcher_Trasmit_State)
				{
				case BCM_TX_DISPATCHER_ID_TX:
				
						gu8_BCM_TxDispatcher_Trasmit_State = BCM_TX_DISPATCHER_SIZEL_TX;

						gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_TRANSMIT_BYTE;
					break;
				
				case BCM_TX_DISPATCHER_SIZEL_TX:
					
					gu8_BCM_TxDispatcher_Trasmit_State = BCM_TX_DISPATCHER_SIZEH_TX;

					gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_TRANSMIT_BYTE;
					break;
					
				case BCM_TX_DISPATCHER_SIZEH_TX:
					
					gu8_BCM_TxDispatcher_Trasmit_State = BCM_TX_DISPATCHER_DATA_TX;

					gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_TRANSMIT_BYTE;
					break;
				
				case BCM_TX_DISPATCHER_DATA_TX:
					
                        / * Loop through TX buffer and transmit each byte * /
                        if(gu8_BCM_TxBuffer_Index < gu16_TxSize)
                        {
                        	/ * Increment index * /
                        	gu8_BCM_TxBuffer_Index++;
                        	gptr_TxBuffer++;
                        
                        	gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_TRANSMIT_BYTE;
                        }
                        else
                        {
                        	gu8_BCM_TxDispatcher_Trasmit_State = BCM_TX_DISPATCHER_ID_TX;
                        
                        	gptr_BCM_TxC_CBK_Func();
                        
                        	/ * Unlock the Tx buffer * /
                        	gu8_BCM_TxBuffer_LockState = BCM_TX_BUFFER_UNLOCKED;
                        
                        	/ * Indicate Send has finished * /
                        	gu8_BCM_SendFlag = BCM_SEND_NOT_TRIGGERED;
                        
                        	/ * Switch BCM_TxDispatcher_State to BCM_DISPATCHER_IDLE * /
                        	gu8_BCM_TxDispatcher_State = BCM_DISPATCHER_IDLE;
                        }
					break;
					
				default: 
						ERR_Status;
				}
			default:
				ERR_Status;
			}
		
	return ERR_Status;
}

*/

