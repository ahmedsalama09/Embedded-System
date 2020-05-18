/********************************************************************************
 *																				*
 * Module : LCD																	*
 *																				*
 * File Name : LCD.h															*
 *																				*
 * Description : Header File for LCD Driver.									*
 *																				*
 * Author : Ahmed Salama														*
 *																				*
 ********************************************************************************/

#ifndef LCD_H_
#define LCD_H_

/********************************************************************************
 * 								  Included Files								*
 ********************************************************************************/

#include "/LCD_APP/MCAL/DIO.h"
#include "util/delay.h"
#include "/LCD_APP/common_macros.h"
#include "/LCD_APP/std_types.h"
#include "/LCD_APP/Error_table.h"
#include "/LCD_APP/SERVICE/LCD_CONFIG.h"

/********************************************************************************
 * 								Preprocessor Macros								*
 ********************************************************************************/

/*
 * LCD pins
 */
#define RS						0x02
#define RW 						0x04
#define E  						0x08
#define LCD_CTRL_PORT_DIR 		GPIOA
#define LCD_CTRL_PORT 			GPIOA
#define LCD_DATA_PORT_DIR 		GPIOA
#define LCD_DATA_PORT 			GPIOA

/*
 * LCD Commands
 */
#define CLEAR_COMMAND					0x01
#define TWO_LINE_LCD_EIGHT_BIT_MODE 	0x30
#define FOUR_BITS_DATA_MODE_ACTIVATED 	0x02
#define TWO_LINE_LCD_FOUR_BIT_MODE 		0x28
#define CURSOR_OFF 						0x0C
#define CURSOR_ON 						0x0E
#define SET_CURSOR_LOCATION 			0x80
#define RETURN_HOME						0x02
/************************************************************************/
/*					CHOOSE YOUR MODE                                    */
/************************************************************************/
#define DATA_BITS_MODE DATA_BITS_4
#if(DATA_BITS_MODE == DATA_BITS_4)
#define UPPER_PORT_BITS
#endif

/********************************************************************************
 * 								Functions Prototypes							*
 ********************************************************************************/

/*
 *Parm : connection mode "8/4 bits mode"
 * Description : A Function to Initialize the LCD Driver.
 *return : Error state : if wrong connection mode was written
 */
ERROR_STATUS LCD_init(uint8_t u8_connection_mode);

/*
 * Description : A Function to send a Command.
 */
ERROR_STATUS LCD_sendCommand(uint8_t command);

/*
 *Parm : byte to be sent
 * Description : A Function to write data (display character).
 *return : Error : not intialized or wrong connection
 */
ERROR_STATUS LCD_displayCharacter(uint8_t data);

/*
 *Parm: pointer to string to be sent
 * Description : A Function to display a string.
 *return : Error if not sent or null ptr
 */
ERROR_STATUS LCD_displayString(const sint8_t* str);

/*
 *Parm : row and column location needed
 * Description : A Function to move the cursor to an exact spot.
 *return : error status if found or E_OK if no error
 */
ERROR_STATUS LCD_goToRowColumn(uint8_t row, uint8_t col);

/*
 *Parm : location at which you want to write it and string to be written
 * Description : A Function to move the cursor to an exact spot and display a string at the spot.
 *return : Error status if found
 */
ERROR_STATUS LCD_displayStringRowColumn(uint8_t row, uint8_t col, const sint8_t* str);

/*
 * Description : A Function to display an integer number.
 */
ERROR_STATUS LCD_displayInteger(sint32_t num);

/*
 * Description : A Function to Clear the LCD Screen.
 */
ERROR_STATUS LCD_clearScreen(void);

#endif
