
/************************************************************************/
/*							INCLUDES                                    */
/************************************************************************/
#include "LCD.h"
#include "util/delay.h"
#include <avr/io.h>
#include "/LCD_APP/MCAL/DIO.h"
#include "/LCD_APP/SERVICE/LowPower.h"

/************************************************************************/
/*							DEFINES                                     */
/************************************************************************/
#define LCD_NOT_INITIALIZED		(0)
#define LCD_INITIALIZED			(1)
#define INT_SIZE				(12)
#define SIGNED_FLAG				(1)
#define CHAR_0					(0)

/************************************************************************/
/*							Global Var.                                 */
/************************************************************************/
static uint8_t gu8_LcdInitState = LCD_NOT_INITIALIZED;
static uint8_t gu8_commandFlag = ZERO;
static uint8_t gu8_DispFlag = ZERO;
static uint8_t gu8_init1st = ZERO;
static uint8_t gu8_TempStrCounter = ZERO;

/********************************************************************************
 * 								Functions Definitions							*
 ********************************************************************************/

/*
 *Parm : connection mode "8/4 bits mode"
 * Description : A Function to Initialize the LCD Driver.
 *return : Error state : if wrong connection mode was written
 */
ERROR_STATUS LCD_init(uint8_t u8_connection_mode)
{
	/*create error flag*/
	ERROR_STATUS ERR_STATE = E_OK;
	
	/*check if LCD initialized before*/
	if(gu8_LcdInitState == LCD_NOT_INITIALIZED)
	{
		if(gu8_init1st == ZERO)
		{
			/*set pins as output
		PB0 : ENABLE PIN , PB1: RS PIN , PB2 : RW PIN*/
		st_DIO_Cfg_t st_dio_init = {.GPIO = LCD_CTRL_PORT , .pins = (LOWER_NIBBLE) , .dir = OUTPUT};
		DIO_init(&st_dio_init);
	
		st_dio_init.GPIO = GPIOA;
		st_dio_init.pins = UPPER_NIBBLE;
		st_dio_init.dir = OUTPUT;
	
		DIO_init(&st_dio_init);
		
		gu8_init1st = 1;
		}
	
	/*init_lcd for 4 bits mode*/
	if(gu8_commandFlag == ZERO)
	{
	
	LCD_sendCommand(FOUR_BITS_DATA_MODE_ACTIVATED);	
	}
	
	else if(gu8_commandFlag == 1)
	{
		LCD_sendCommand(TWO_LINE_LCD_FOUR_BIT_MODE);
			
	}
	
	else if(gu8_commandFlag == 2)
	{
		/*turn off cursor and clear LCD*/
		LCD_sendCommand(CURSOR_OFF);	
	}
	
	else if(gu8_commandFlag == 3)
	{
		LCD_sendCommand(CLEAR_COMMAND);
		
		
		gu8_commandFlag = ZERO;	
		/*save initialized state*/
		gu8_LcdInitState = LCD_INITIALIZED;	
	}
	else if(gu8_commandFlag == 4)
	{
		gu8_commandFlag = ZERO;
		
		gu8_LcdInitState = LCD_INITIALIZED;
	}

	}
	/*if initialized before then save multiple init*/
	else
	{
		ERR_STATE = ERROR_LCD_MULTIPLE_INITIALIZATION;
	}
		
	/*return error state*/
	return ERR_STATE;
}


/*
 * Description : A Function to send a Command.
 */
ERROR_STATUS LCD_sendCommand(uint8_t command)
{
	/*create error flag*/
	ERROR_STATUS ERR_STATE = E_OK;
	
	/*create delay counter*/
	static uint8_t au8_delay_ticks = ZERO;
	DIO_Write(GPIOA, RS | RW , LOW);
		
 	/*set enable pin to start writing*/
 	DIO_Write(LCD_CTRL_PORT, E , HIGH);

	/*write the upper nibble of you data*/
	DIO_Write(LCD_DATA_PORT , (UPPER_NIBBLE ), (command & UPPER_NIBBLE));

 	DIO_Write(LCD_CTRL_PORT, E , LOW);

 	DIO_Write(LCD_CTRL_PORT, E , HIGH);

	/*write the lower nibble of you data*/
	DIO_Write(LCD_DATA_PORT , UPPER_NIBBLE, ((command & LOWER_NIBBLE) << DATA_BITS_4));	

	/*clear enable pin*/
 	DIO_Write(LCD_CTRL_PORT, E , LOW);

	gu8_commandFlag++;

	
	return ERR_STATE;
	
}

/*
 *Parm : byte to be sent
 * Description : A Function to write data (display character).
 *return : Error : not intialized or wrong connection
 */
ERROR_STATUS LCD_displayCharacter(uint8_t data)
{
	/*create error flag*/
	ERROR_STATUS ERR_STATE = E_OK;
	

 	if(gu8_LcdInitState == LCD_INITIALIZED)
 	{
				
		/*set RS for data mode and RW : Write mode*/
		DIO_Write(LCD_CTRL_PORT, RS, HIGH);
		DIO_Write(LCD_CTRL_PORT, RW, LOW);
		
		DIO_Write(LCD_CTRL_PORT, E, HIGH);
		
		DIO_Write(LCD_DATA_PORT, UPPER_NIBBLE, (data & UPPER_NIBBLE));

		DIO_Write(LCD_CTRL_PORT, E, LOW);
				
		DIO_Write(LCD_CTRL_PORT, E, HIGH);
	
		DIO_Write(LCD_DATA_PORT, UPPER_NIBBLE, (data & LOWER_NIBBLE) << DATA_BITS_4);
		
		DIO_Write(LCD_CTRL_PORT, E, LOW);
		gu8_DispFlag++;
		
		}
	
	return ERR_STATE;
}

/*
 *Parm: pointer to string to be sent
 * Description : A Function to display a string.
 *return : Error if not sent or null ptr
 */
ERROR_STATUS LCD_displayString(const sint8_t* str)
{
	/*create error flag*/
	ERROR_STATUS ERR_STATE = E_OK;
	
	if(str == NULL)
	{
		ERR_STATE = ERROR_LCD_NULL_POINTER;
	}
	else
	{
		/*loop till end of string*/
		while (*str != '\0')
 		{
		LCD_displayCharacter(*str);
		_delay_us(40);
		str++;
		 }
	}
	
	return ERR_STATE;
}

/*
 *Parm : row and column location needed
 * Description : A Function to move the cursor to an exact spot.
 *return : error status if found or E_OK if no error
 */
ERROR_STATUS LCD_goToRowColumn(uint8_t row, uint8_t col)
{
	/*create error flag*/
	ERROR_STATUS ERR_STATE = E_OK;
	uint8_t address = ZERO;
	
	
	/*check if lcd init or not*/
	if(gu8_LcdInitState == LCD_INITIALIZED)
	{
		switch(row)
		{
			case 0 :
				address = col;
			break;
			
			case 1 :
				address = col + 0x40;
			break;
			
			case 2 :
				address = col + 0x10;
			break;
			
			case 3 :
				address = col + 0x50;
			break;
			
			default:
				ERR_STATE = ERROR_LCD_WRONG_LOCATION;
			break;
		}
		ERR_STATE = LCD_sendCommand(address | SET_CURSOR_LOCATION);
	}
	
	else
	{
		ERR_STATE = ERROR_LCD_NOT_INITIALIZED;
	}
	
	return ERR_STATE;
}

/*
 *Parm : location at which you want to write it and string to be written
 * Description : A Function to move the cursor to an exact spot and display a string at the spot.
 *return : Error status if found
 */
ERROR_STATUS LCD_displayStringRowColumn(uint8_t row, uint8_t col, const sint8_t* str)
{
	/*create error flag*/
	ERROR_STATUS ERR_STATE = E_OK;
	
	ERR_STATE = LCD_goToRowColumn(row, col);
	_delay_us(30);
	ERR_STATE = LCD_displayString(str);
	
	return ERR_STATE;
}

/*
 * Description : A Function to display an integer number.
 */
ERROR_STATUS LCD_displayInteger(sint32_t num)
{
	/*create error flag*/
	ERROR_STATUS ERR_STATE = E_OK;
	
	
	sint8_t buff[INT_SIZE];
	sint8_t* ptr = &buff[INT_SIZE-1]; 				// char *ptr = (buff + 11); [] is another way of describing the dereferencing operator
	char N_flag = ZERO;
	
	if(num < ZERO)
	{
		N_flag = SIGNED_FLAG;
		num = -num;
	}
	
	*ptr = '\0';
	
	do{
	    *--ptr = (num % 10) + CHAR_0;
	    num /= 10;
	}while(num != ZERO);
	
	if(N_flag == SIGNED_FLAG)
		*--ptr = '-';
		
	ERR_STATE = LCD_displayString(ptr);
	
	return ERR_STATE;
}

/*
 * Description : A Function to Clear the LCD Screen.
 */
ERROR_STATUS LCD_clearScreen(void)
{
	/*create error flag*/
	ERROR_STATUS ERR_STATE = E_OK;
	
	/*send clear command*/
	ERR_STATE = LCD_sendCommand(CLEAR_COMMAND); 
	
	
	return ERR_STATE;
}
