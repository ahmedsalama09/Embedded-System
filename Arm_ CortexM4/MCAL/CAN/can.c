/*
 * can.c
 *
 *  Created on: Jun 8, 2020
 *      Author: ahmed
 */

/*Includes*/
#include "CANlcfg.h"
#include "can.h"
#include <MCAL/PerRegs.h>

/*Macros*/
#define CAN_ST	arrCan[au8_Loop]

#define NOT_INIT (0u)
#define MODULE_INIT (1u)

/*Can CLOCK Macros*/
#define CAN_CLOCK_BASE 		(0x2000000u)

#define CAN2_OFFSET_ADDR 	(0x400u)

/*Masks defines and shifts*/
#define SLEEP_MASK 			(0x2u)
#define INIT_MASK 			(0x1u)
#define PRIORITY_SHIFT		(2u)
#define LOCK_SHIFT			(3u)
#define RETRANSMIT_SHIFT	(4u)
#define AUTOWAKE_SHIFT		(5u)
#define BUSOFF_SHIFT		(6u)
#define TIME_TRIG_SHIFT		(7U)
#define DEBUG_SHIFT			(16u)

/****IRQ SHIFTS*****/
#define TX_IRQ_SHIFT		(0u)
#define PEND0_IRQ_SHIFT		(1u)
#define FULL0_IRQ_SHIFT		(2u)
#define OVERRUN0_IRQ_SHIFT	(3u)
#define PEND1_IRQ_SHIFT		(4u)
#define FULL1_IRQ_SHIFT		(5u)
#define OVERRUN1_IRQ_SHIFT	(6u)
#define WARN_IRQ_SHIFT		(8u)
#define PASSIVE_IRQ_SHIFT	(9u)
#define BOFF_IRQ_SHIFT		(10u)
#define LAST_IRQ_SHIFT		(11u)
#define ERROR_IRQ_SHIFT		(15u)
#define WAKE_IRQ_SHIFT		(16u)
#define SLEEP_IRQ_SHIFT		(17u)

/*BIT Time SHIFT*/
#define BAUDRATE_SHIFT		(0u)
#define TIMESEG_1_SHIFT		(16u)
#define TIMESEG_2_SHIFT		(20u)
#define JUMP_WIDTH_SHIFT	(24u)
#define LOOPBACK_SHIFT		(30u)
#define SILENT_MODE_SHIFT	(31u)

/*Filter Macros*/
#define FILTER_INIT_VALUE	(0x3F01u)
#define FILTER_DUAL_MASK_NUM (2u)
#define FILTER_DUAL_LIST_NUM (4u)
#define DUAL_EX_ID			 (7u)
#define FILTER_IDE_SHIFT	 (3u)
#define FILTER_RTR_SHIFT	 (4u)
#define FILTER_ID_SHIFT		 (5u)
#define FILTER_MASK_SHIFT	 (21u)
#define FILTER_BIT_SHIFT	 (16u)

/*Tx Macros*/
#define ADDR_OFFSET 		 (0x400u)
#define TRANSMIT_REQUEST	 (0x1u)
#define TX_RTR_SHIFT		 (1U)
#define TX_IDE_SHIFT		 (2U)
#define TX_EXT_SHIFT		 (3U)
#define TX_STD_SHIFT		 (21U)
#define MAILBOX0_EMPTY		 (0x4000000U)
#define MAILBOX1_EMPTY		 (0x8000000U)
#define MAILBOX2_EMPTY		 (0x10000000U)
#define MAX_BYTES_NO		 (8u)
#define BYTE_SHIFT			 (8u)
#define REG_MAX_BYTES		 (4u)

/*Rx Macros*/
#define PENDING_MSG_MASK	 (3u)
#define ID_MASK				 (4u)
#define EXTENDED_ID_MASK	 (0xFFFFFFF8U)
#define STD_ID_MASK			 (0xFFE00000U)
#define REGS_MSGS_NO		 (4u)
#define RELEASE_MAILBOX		 (16u)
#define MSG_SIZE_MASK		 (15u)

/*array of buffers macros*/
#define SIZE_INDEX			 (1u)
#define MASK_INDEX			 (0u)

#define BUF_MASK_MODE			 (1u)
#define BUF_LIST_MODE			 (2u)
#define BUF_NOT_DEFINED_MODE	 (0u)

#define BUF_FULL_SIZE			 (2u)
#define BUF_HALF_SIZE			 (1u)
#define BUF_EMPTY_SIZE			 (0u)
#define BUF_QUART_SIZE			 (3u)
#define BUF_3_QUART_SIZE		 (4u)

#define RTR_FILTER_32_SHIFT		 (1u)
#define IDE_FILTER_32_SHIFT		 (2u)
#define IDENT_FILTER_32_SHIFT	 (3u)

#define IDE_FILTER_16_SHIFT		 (3u)
#define RTR_FILTER_16_SHIFT		 (4u)
#define ID_FILTER_16_SHIFT		 (5u)
#define MASK_FILTER_16_SHIFT	 (16u)

#define ID_2_OFFSET				(16u)

/*Globals*/
static uint8_t garr_Init_State[2] = {NOT_INIT , NOT_INIT};
static uint8_t gu8_Tx_Mail = 0;

str_RxMsg_t arr_msgs[RX_MAX_ARRAY_SIZE] = {0};


/*Can init function
 * this is a linked config file so search for the canlcfg.c file to configure what to write in can
 * input : None but externed struct used
 * output : Error if found
 */
ERROR_STATUS CanInit(void)
{
	/*Create error variable*/
	ERROR_STATUS ERR_STATE = E_NOK;

	/*check if module init before or not*/
	if((garr_Init_State[0] == MODULE_INIT) || (garr_Init_State[1] == MODULE_INIT))
	{
		ERR_STATE = ERROR_CAR_BASE + ERROR_MULTIPLE_MODULE_INIT;
	}
	else
	{
		/*Create local loop var*/
		uint8_t au8_Loop = 0;
		uint8_t au8_BankLoop=0;;

		/*Loop to configure all structs*/
		for(au8_Loop = 0; au8_Loop<NUM_OF_CONFIGS ; au8_Loop++)
		{
			/*Set init value as module init*/
			garr_Init_State[CAN_ST.CAN_ID] = MODULE_INIT;

			/*First init Module CLOCK*/
			RCC->APB1ENR |= (CAN_ST.CAN_ID + CAN_CLOCK_BASE);

			/*Define can unit to use*/
			str_CAN_t *CanPtr = (str_CAN_t *)((CAN_ST.CAN_ID * (CAN2_OFFSET_ADDR)) + CAN1_BASE);

			/************************MCR REGS****************************/

			/*Clear reg*/
			CanPtr->MCR =0;

			/*Send init REQ*/
			CanPtr->MCR |= (INIT_MASK);

			/*Set priority order*/
			CanPtr->MCR |= (CAN_ST.TRANSMIT_PRIORITY << PRIORITY_SHIFT);

			/*Set LOCK mode*/
			CanPtr->MCR |= (CAN_ST.FIFO_LOCK << LOCK_SHIFT);

			/*Auto transmition*/
			CanPtr->MCR |= (CAN_ST.AUTO_RETRANSMIT << RETRANSMIT_SHIFT);

			/*auto wakeup*/
			CanPtr->MCR |= (CAN_ST.AUTO_WAKEUP << AUTOWAKE_SHIFT);

			/*Auto busoff*/
			CanPtr->MCR |= (CAN_ST.BUS_OFF << BUSOFF_SHIFT);

			/*TimeTrigger mode*/
			CanPtr->MCR |= (CAN_ST.TIME_TRIGGER_MODE << TIME_TRIG_SHIFT);

			/*DEBUG FREEZE*/
			CanPtr->MCR |= (CAN_ST.DEBUG_STATE << DEBUG_SHIFT);

			/*********************INTERRUPTS******************/
			CanPtr->IER = 0;

			CanPtr->IER |= (CAN_ST.BOFF_IRQ << BOFF_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.ERR_PASSIVE_IRQ << PASSIVE_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.ERROR_IRQ << ERROR_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.ERR_WARN_IRQ << WARN_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.FIFO_PEND_IRQ_0 << PEND0_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.FIFO_PEND_IRQ_1 << PEND1_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.FULL_FIFI_IRQ_0 << FULL0_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.FULL_FIFI_IRQ_1 << FULL1_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.LAST_ERROR_IRQ << LAST_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.OVERRUN_IRQ_0 << OVERRUN0_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.OVERRUN_IRQ_1 << OVERRUN1_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.SLEEP_IRQ << SLEEP_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.TX_EMPTY_IRQ << TX_IRQ_SHIFT);
			CanPtr->IER |= (CAN_ST.WAKE_IRQ << WAKE_IRQ_SHIFT);

			/******************BIT TIME**********************/
			/*Clear reg*/
			CanPtr->BTR = 0;

			CanPtr->BTR |= (CAN_ST.BAUD_RATE << BAUDRATE_SHIFT);
			CanPtr->BTR |= (CAN_ST.TIME_SEG_1 << TIMESEG_1_SHIFT);
			CanPtr->BTR |= (CAN_ST.TIME_SEG_2 << TIMESEG_2_SHIFT);
			CanPtr->BTR |= (CAN_ST.RESYNCH_JUMP << JUMP_WIDTH_SHIFT);

			CanPtr->BTR |= (CAN_ST.SILENT_MODE << SILENT_MODE_SHIFT);
			CanPtr->BTR |= (CAN_ST.LOOP_BACK_MODE << LOOPBACK_SHIFT);

			/******************FILTER BANKS***************/
			/*Set filter init mode*/
			CanPtr->FMR = FILTER_INIT_VALUE;

			/*Clear FM1R*/
			CanPtr->FM1R = 0;

			/*Loop for filters*/
			for(au8_BankLoop=0 ; au8_BankLoop<NUM_OF_FILTERS ; au8_BankLoop++)
			{
				/*Buffer array for filter reg state*/
				uint8_t arr_FilterState[2][28] = {0};

				uint8_t BufLoop = 0;

				/*loop at the buffer*/
				for(BufLoop = 0 ; BufLoop < 28 ;BufLoop++)
				{
					/*Define for state to allocate filter*/
					if(CAN_ST.Dual_Mode == SINGLE_32BIT_CONF)
					{
						if(CAN_ST.FILTER_MODE == FILTER_MASK_MODE)
						{
							/*Check if buffer empty then allocate it in this bank*/
							if(arr_FilterState[SIZE_INDEX][BufLoop] == BUF_EMPTY_SIZE)
							{
								/*Activate filte init moder*/
								CanPtr->FM1R |= (1 << BufLoop);

								/*Choose list or identifier mode*/
								CanPtr->FM1R |= ((CAN_ST.FILTER_MODE) << BufLoop);

								/*Choose dual or 16bit mode*/
								CanPtr->FM1R |= ((CAN_ST.Dual_Mode) << BufLoop);

								/*Write registers in filter reg*/
								CanPtr->FM1R |= ((CAN_ST.FIFO_USED) << BufLoop);

								CanPtr->FILTER[au8_Loop].FR1 = 0;
								CanPtr->FILTER[au8_Loop].FR2 = 0;

								/*Allocate filter*/
								CanPtr->FILTER[BufLoop].FR1 = (CAN_ST.filter_conf[au8_BankLoop].ID
								<< IDENT_FILTER_32_SHIFT);

								/*Write RTR and IDE*/
								CanPtr->FILTER[BufLoop].FR1 |= (((CAN_ST.filter_conf[au8_BankLoop].RTR) << RTR_FILTER_32_SHIFT)
										| ((CAN_ST.filter_conf[au8_BankLoop].IDE) << IDE_FILTER_32_SHIFT));

								/*Allocate Mask*/
								CanPtr->FILTER[BufLoop].FR2 = CAN_ST.filter_conf[au8_BankLoop].Mask;

								/*Assign filter full and mask in buffer*/
								arr_FilterState[0][BufLoop] = BUF_MASK_MODE;
								arr_FilterState[1][BufLoop] = BUF_FULL_SIZE;

								/*Activate Filter*/
								CanPtr->FM1R &=~ (1 << BufLoop);
							}
						}

						/*Check for FILTER_LIST_MODE*/
						else if(CAN_ST.FILTER_MODE == FILTER_IDLIST_MODE)
						{
							/*Check for buffer which isn't masked*/
							if(arr_FilterState[MASK_INDEX][BufLoop] != BUF_MASK_MODE)
							{
								/*check if it's list and not full or empty one*/
								if((arr_FilterState[MASK_INDEX][BufLoop] == BUF_LIST_MODE) &&
									(arr_FilterState[SIZE_INDEX][BufLoop] != BUF_FULL_SIZE))
									{
										/*Activate filte init moder*/
										CanPtr->FM1R |= (1 << BufLoop);

										/*Choose list or identifier mode*/
										CanPtr->FM1R |= ((CAN_ST.FILTER_MODE) << BufLoop);

										/*Choose dual or 16bit mode*/
										CanPtr->FM1R |= ((CAN_ST.Dual_Mode) << BufLoop);

										/*Write registers in filter reg*/
										CanPtr->FM1R |= ((CAN_ST.FIFO_USED) << BufLoop);

										/*Allocate ID In mask Regs*/
										CanPtr->FILTER[BufLoop].FR2 = (CAN_ST.filter_conf[au8_BankLoop].ID
																	<< IDENT_FILTER_32_SHIFT);

										/*Write RTR and IDE*/
										CanPtr->FILTER[BufLoop].FR2 |= (((CAN_ST.filter_conf[au8_BankLoop].RTR)
																		<< RTR_FILTER_32_SHIFT)
												| ((CAN_ST.filter_conf[au8_BankLoop].IDE) << IDE_FILTER_32_SHIFT));

										/*Assign full size to buf*/
										arr_FilterState[SIZE_INDEX][BufLoop] = BUF_FULL_SIZE;

										/*Activate Filter*/
										CanPtr->FM1R &=~ (1 << BufLoop);
									}
								else
								{
									/*Activate filte init moder*/
									CanPtr->FM1R |= (1 << BufLoop);

									/*Choose list or identifier mode*/
									CanPtr->FM1R |= ((CAN_ST.FILTER_MODE) << BufLoop);

									/*Choose dual or 16bit mode*/
									CanPtr->FM1R |= ((CAN_ST.Dual_Mode) << BufLoop);

									/*Write registers in filter reg*/
									CanPtr->FM1R |= ((CAN_ST.FIFO_USED) << BufLoop);

									/*Allocate ID In ID Regs*/
									CanPtr->FILTER[BufLoop].FR1 = (CAN_ST.filter_conf[au8_BankLoop].ID
																<< IDENT_FILTER_32_SHIFT);
									/*Write RTR and IDE*/
									CanPtr->FILTER[BufLoop].FR1 |= (((CAN_ST.filter_conf[au8_BankLoop].RTR)
																	<< RTR_FILTER_32_SHIFT)
											| ((CAN_ST.filter_conf[au8_BankLoop].IDE) << IDE_FILTER_32_SHIFT));

									/*Assign list mode and half buf*/
									arr_FilterState[SIZE_INDEX][BufLoop] = BUF_HALF_SIZE;
									arr_FilterState[MASK_INDEX][BufLoop] = BUF_LIST_MODE;

									/*Activate Filter*/
									CanPtr->FM1R &=~ (1 << BufLoop);
								}
							}
						}
					}
					else if((CAN_ST.Dual_Mode == DUAL_16BIT_CONF) &&
							(CAN_ST.FILTER_MODE == FILTER_MASK_MODE) &&
							(arr_FilterState[SIZE_INDEX][BufLoop] != BUF_FULL_SIZE))
					{
						/*Create ptr to frame*/
						uint32_t *Fil_Ptr = 0;

						/*Check for half full buffer*/
						if(arr_FilterState[SIZE_INDEX][BufLoop] == BUF_HALF_SIZE)
						{
							Fil_Ptr = (uint32_t *)&(CanPtr->FILTER[BufLoop].FR2);

							arr_FilterState[SIZE_INDEX][BufLoop] = BUF_FULL_SIZE;
						}
						else if(arr_FilterState[SIZE_INDEX][BufLoop] == BUF_EMPTY_SIZE)
						{
							Fil_Ptr = (uint32_t *)&(CanPtr->FILTER[BufLoop].FR1);

							arr_FilterState[SIZE_INDEX][BufLoop] = BUF_HALF_SIZE;
						}
							/*Activate filte init moder*/
							CanPtr->FM1R |= (1 << BufLoop);

							/*Choose list or identifier mode*/
							CanPtr->FM1R |= ((CAN_ST.FILTER_MODE) << BufLoop);

							/*Choose dual or 16bit mode*/
							CanPtr->FM1R |= ((CAN_ST.Dual_Mode) << BufLoop);

							/*Write registers in filter reg*/
							CanPtr->FM1R |= ((CAN_ST.FIFO_USED) << BufLoop);

							/*Write Values in mask reg*/
							*Fil_Ptr = ((CAN_ST.filter_conf[au8_BankLoop].IDE << IDE_FILTER_16_SHIFT)
									| (CAN_ST.filter_conf[au8_BankLoop].RTR << RTR_FILTER_16_SHIFT)
									| (CAN_ST.filter_conf[au8_BankLoop].ID << ID_FILTER_16_SHIFT)
									| (CAN_ST.filter_conf[au8_BankLoop].Mask << MASK_FILTER_16_SHIFT));

							/*Enable Filter*/
							CanPtr->FM1R &=~ (1 << BufLoop);
					}
					else if((CAN_ST.Dual_Mode == DUAL_16BIT_CONF) &&
							(CAN_ST.FILTER_MODE == FILTER_IDLIST_MODE) &&
							(arr_FilterState[SIZE_INDEX][BufLoop] != BUF_FULL_SIZE) &&
							(arr_FilterState[MASK_INDEX][BufLoop] != BUF_MASK_MODE))
					{
						/*Create ptr to frame*/
						uint32_t *Fil_Ptr = 0;

						uint8_t ID_Shift =0 , RTR_Shift = 0, IDE_Shift =0;

						/*Activate filte init moder*/
						CanPtr->FM1R |= (1 << BufLoop);

						/*Choose list or identifier mode*/
						CanPtr->FM1R |= ((CAN_ST.FILTER_MODE) << BufLoop);

						/*Choose dual or 16bit mode*/
						CanPtr->FM1R |= ((CAN_ST.Dual_Mode) << BufLoop);

						/*Write registers in filter reg*/
						CanPtr->FM1R |= ((CAN_ST.FIFO_USED) << BufLoop);

						/*Check for Empty Buf*/
						if(arr_FilterState[SIZE_INDEX][BufLoop] < BUF_HALF_SIZE)
						{
							Fil_Ptr =(uint32_t *)&(CanPtr->FILTER[BufLoop].FR1);

							if(arr_FilterState[SIZE_INDEX][BufLoop] == BUF_EMPTY_SIZE)
							{
								arr_FilterState[SIZE_INDEX][BufLoop] = BUF_QUART_SIZE;

								ID_Shift = ID_FILTER_16_SHIFT;

								IDE_Shift = IDE_FILTER_16_SHIFT;
								RTR_Shift = RTR_FILTER_16_SHIFT;
							}
							else if(arr_FilterState[SIZE_INDEX][BufLoop] == BUF_QUART_SIZE)
							{
								arr_FilterState[SIZE_INDEX][BufLoop] = BUF_HALF_SIZE;

								ID_Shift = (ID_FILTER_16_SHIFT  + ID_2_OFFSET);

								IDE_Shift = (IDE_FILTER_16_SHIFT + ID_2_OFFSET);
								RTR_Shift = (RTR_FILTER_16_SHIFT + ID_2_OFFSET);
							}
						}
						else
						{
							Fil_Ptr =(uint32_t *)&(CanPtr->FILTER[BufLoop].FR2);

							if(arr_FilterState[SIZE_INDEX][BufLoop] == BUF_HALF_SIZE)
							{
								arr_FilterState[SIZE_INDEX][BufLoop] = BUF_3_QUART_SIZE;

								ID_Shift = ID_FILTER_16_SHIFT;

								IDE_Shift = IDE_FILTER_16_SHIFT;
								RTR_Shift = RTR_FILTER_16_SHIFT;
							}
							else if(arr_FilterState[SIZE_INDEX][BufLoop] == BUF_QUART_SIZE)
							{
								arr_FilterState[SIZE_INDEX][BufLoop] = BUF_FULL_SIZE;

								ID_Shift = (ID_FILTER_16_SHIFT + 16);

								IDE_Shift = (IDE_FILTER_16_SHIFT + 16);
								RTR_Shift = (RTR_FILTER_16_SHIFT + 16);
							}
						}
						/*Write Values in mask reg*/
						*Fil_Ptr = ((CAN_ST.filter_conf[au8_BankLoop].IDE << IDE_Shift)
								| (CAN_ST.filter_conf[au8_BankLoop].RTR << RTR_Shift)
								| (CAN_ST.filter_conf[au8_BankLoop].ID << ID_Shift));
						/*Enable Filter*/
						CanPtr->FM1R &=~ (1 << BufLoop);
					}
				}
			}

			/*Turn normal mode on*/
			CanPtr->MCR &= ~ INIT_REQUEST_SET;

			/*Wait for normal mode On*/
			while((CanPtr->MSR & INIT_MASK) != 0);
		}

	}
	return ERR_STATE;
}

/*Send data through can function
 * Description : a function used to configure a full frame for can tx to be sent
 * Input : RTR  , IDE , Identifier and data to be sent
 * Output : Error if found
 */
ERROR_STATUS Can_Tx(uint8_t IDE, uint8_t RTR , uint32_t ID, uint8_t *Data , uint8_t Can_Num , uint8_t BytesNo)
{
	ERROR_STATUS ERR_STATE = E_NOK;

	/*Check if module was init or not*/
	if(garr_Init_State[Can_Num] == NOT_INIT)
	{
		ERR_STATE = ERROR_CAN_BASE + ERROR_MODULE_NOT_INIT;
	}
	else
	{
		/*Check for parameters*/
		if( (IDE > EXTENDED_ID) ||
		    (RTR > REMOTE_FRAME)||
			(Data  == NULL)		||
			(BytesNo > MAX_BYTES_NO) ||
			(Can_Num > CAN_2))
		{
			ERR_STATE = ERROR_CAN_BASE + ERROR_INVALID_PARAMETERS;
		}
		else
		{
			/*Create address variable*/
			volatile str_CAN_t *PtrCan  = (str_CAN_t *)((uint32_t)CAN1 + (Can_Num * ADDR_OFFSET));

			/*Check for empty mailbox*/
			if((PtrCan->TSR | MAILBOX0_EMPTY) != 0 )
			{
				gu8_Tx_Mail =0;
			}
			else if((PtrCan->TSR | MAILBOX1_EMPTY) != 0 )
			{
				gu8_Tx_Mail =1;
			}
			else if((PtrCan->TSR | MAILBOX2_EMPTY) != 0 )
			{
				gu8_Tx_Mail =2;
			}
			else
			{
				ERR_STATE = ERROR_CAN_BASE + NO_EMPTY_MAILBOX;
			}
			if(ERR_STATE != (ERROR_CAN_BASE + NO_EMPTY_MAILBOX))
			{
				/*Bytes_LoopVariable*/
				uint8_t au8_ByteLoop=0;

				/*Write IDE And RTR Values*/
				PtrCan->TX[gu8_Tx_Mail].TIR = 0 ;

				PtrCan->TX[gu8_Tx_Mail].TIR |= (IDE << TX_IDE_SHIFT) | (RTR << TX_RTR_SHIFT);

				/*Check if ide std or ext*/
				if(IDE == STANDARD_ID)
				{
					PtrCan->TX[gu8_Tx_Mail].TIR |= (ID << TX_STD_SHIFT);
				}
				else if(IDE == EXTENDED_ID)
				{
					PtrCan->TX[gu8_Tx_Mail].TIR |= (ID << TX_EXT_SHIFT);
				}

				/*Write Data Length*/
				PtrCan->TX[gu8_Tx_Mail].TDTR  = BytesNo;

				for(au8_ByteLoop = 0 ; au8_ByteLoop < BytesNo ; au8_ByteLoop++)
				{
					if(au8_ByteLoop < 4)
					{
						PtrCan->TX[gu8_Tx_Mail].TDLR = (Data[au8_ByteLoop] << (BYTE_SHIFT * au8_ByteLoop));
					}
					else
					{
						PtrCan->TX[gu8_Tx_Mail].TDHR = (Data[au8_ByteLoop] <<
								(BYTE_SHIFT * (au8_ByteLoop - REG_MAX_BYTES)));
					}
				}
					/*Send Tx Request*/
					PtrCan->TX[gu8_Tx_Mail].TIR |= TRANSMIT_REQUEST;
			}
		}
	}
	return ERR_STATE;
}

/*Reception
 * Description : Function To receive data from Can receiver
 * note : please configure CAn pins before using
 * Input : Data pointer , Data length
 * Output : Error if found
 */
ERROR_STATUS Can_Rx(uint8_t Can_ID)
{
	/*Create error flag*/
	ERROR_STATUS ERR_STATE = E_NOK;

	/*Check if module init or not*/
	if(garr_Init_State[Can_ID] == NOT_INIT)
	{
		ERR_STATE = ERROR_CAN_BASE + ERROR_MODULE_NOT_INIT;
	}
	else
	{
		/*Check for parameters*/
		if(Can_ID > CAN_2)
		{
			ERR_STATE = ERROR_CAN_BASE + ERROR_WRONG_MODULE;
		}
		else
		{
			/*Define can id ptr*/
			volatile str_CAN_t *PtrCan  = (str_CAN_t *)((uint32_t)CAN1 + (Can_ID * ADDR_OFFSET));

			/*Read Pending FIFOS*/
			uint8_t u8_PendingMailbox_0 = (PtrCan->RF0R & PENDING_MSG_MASK);
			uint8_t u8_PendingMailbox_1 = (PtrCan->RF1R & PENDING_MSG_MASK);

			uint8_t au8_Msgloop = 0;
			uint8_t au8_BytesLoop = 0;

			for(au8_Msgloop = 0 ; au8_Msgloop < u8_PendingMailbox_0 ; au8_Msgloop++)
			{
				/*Save msg length in struct*/
				arr_msgs[au8_Msgloop].msg_size = (PtrCan->RX[0].RDTR & MSG_SIZE_MASK);

				/*First save id and it's type*/
				if((PtrCan->RX[0].RIR & ID_MASK) != 0)
				{
					/*Save ID EX state*/
					arr_msgs[au8_Msgloop].ID_Type = EXTENDED_ID;

					arr_msgs[au8_Msgloop].ID = ((PtrCan->RX[0].RIR) & (EXTENDED_ID_MASK)) >> TX_EXT_SHIFT;
				}
				else
				{
				/*Save ID st state*/
			    arr_msgs[au8_Msgloop].ID_Type = STANDARD_ID;

			    arr_msgs[au8_Msgloop].ID = ((PtrCan->RX[0].RIR) & (STD_ID_MASK)) >> TX_STD_SHIFT;
				}

				/*Loop for bytes*/
				for(au8_BytesLoop = 0 ; au8_BytesLoop < arr_msgs[au8_Msgloop].msg_size ; au8_BytesLoop++)
				{
					if(au8_BytesLoop < REGS_MSGS_NO)
					{
						/*loop for data*/
						arr_msgs[au8_Msgloop].msg[au8_BytesLoop] = PtrCan->RX[0].RDLR[au8_BytesLoop];
					}
					else
					{
						arr_msgs[au8_Msgloop].msg[au8_BytesLoop] = PtrCan->RX[0].RDHR[au8_BytesLoop - 4];
					}
				}

				/*Release MailBox*/
				PtrCan->RF0R |= RELEASE_MAILBOX;

			}

			for(au8_Msgloop = 0 ; au8_Msgloop < u8_PendingMailbox_1 ; au8_Msgloop++)
			{
				/*Save msg length in struct*/
				arr_msgs[au8_Msgloop + 3].msg_size = (PtrCan->RX[1].RDTR & MSG_SIZE_MASK);

				/*First save id and it's type*/
				if((PtrCan->RX[1].RIR & ID_MASK) != 0)
				{
					/*Save ID EX state*/
					arr_msgs[au8_Msgloop + 3].ID_Type = EXTENDED_ID;

					arr_msgs[au8_Msgloop + 3].ID = ((PtrCan->RX[1].RIR) & (EXTENDED_ID_MASK)) >> TX_EXT_SHIFT;
				}
				else
				{
				/*Save ID st state*/
			    arr_msgs[au8_Msgloop + 3].ID_Type = STANDARD_ID;

			    arr_msgs[au8_Msgloop + 3].ID = ((PtrCan->RX[1].RIR) & (STD_ID_MASK)) >> TX_STD_SHIFT;
				}

				/*Loop for bytes*/
				for(au8_BytesLoop = 0 ; au8_BytesLoop < arr_msgs[au8_Msgloop + 3].msg_size ; au8_BytesLoop++)
				{
					if(au8_BytesLoop < REGS_MSGS_NO)
					{
						/*loop for data*/
						arr_msgs[au8_Msgloop + 3].msg[au8_BytesLoop] = PtrCan->RX[1].RDLR[au8_BytesLoop];
					}
					else
					{
						arr_msgs[au8_Msgloop + 3].msg[au8_BytesLoop] = PtrCan->RX[1].RDHR[au8_BytesLoop - 4];
					}
				}

				/*Release MailBox*/
				PtrCan->RF1R |= RELEASE_MAILBOX;

			}
		}
	}
	return ERR_STATE;
}
