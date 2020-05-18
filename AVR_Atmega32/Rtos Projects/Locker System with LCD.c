#include "FreeRTOS.h"
#include "task.h"
#include "/LCD_APP/MCAL/DIO.h"
#include "/LCD_APP/SERVICE/LCD.h"
#include "my_keypad.h"


/************************************************************************/
/*							DEFINES                                     */
/************************************************************************/
#define PERIORITY_1 (1)
#define PERIORITY_2 (2)
#define PERIORITY_3 (3)

#define STACK_100_WORD (100)

#define NO_PARAMETERS (1)

#define NULL_CHAR ('0')
#define MAX_CHAR_NUM (7)

#define DELAY_2_SECONDS (2000)

#define Paramter ((str_Task_t *)pvParameters)

#define PASS_MATCHED (6)
/************************************************************************/
/*							TASK Varibales                              */
/************************************************************************/
typedef struct{
	char Chr_arr[7];
	uint8_t Char_Num;
	uint8_t Tmp;
	const char Pass[6]
	}str_Task_t;
	
str_Task_t str_Task = {.Chr_arr = {0} ,.Char_Num = 0 , .Tmp=0 , .Pass = {'1','2','3','4','5','6'}};



/************************************************************************/
/*								APIS                                    */
/************************************************************************/
/*Create Dio Struct*/
st_DIO_Cfg_t Dio_init;

/*LCDInit
*Description : Task To init LCD once then Suspended
..It has the highest priority
*/
void LCD_Init(void * pvParameters)
{
	while(1)
	{
	LCD_init(DATA_BITS_4);

	vTaskSuspend(NULL);
	}
}

/*KeyPad Task
*Description : Task to read input from Keypad Every configure time
*/
void KeyPad(void * pvParameters)
{
	 TickType_t xLastWakeTime;
	 const TickType_t xFrequency = 130;

	 // Initialize the xLastWakeTime variable with the current time.
	 xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		if(Key_Pressed() != DEFAULT_CHAR && ((Paramter->Char_Num) < MAX_CHAR_NUM)){
			
			Paramter->Chr_arr[Paramter->Char_Num] = (Key_Pressed() + NULL_CHAR);

			(Paramter->Char_Num)++;
			
		}
		vTaskDelayUntil(&xLastWakeTime , xFrequency);
		
	}
}

void LCD_Write(void * pvParameters)
{
	uint8_t Cmp_Res = 0;
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 140;

	// Initialize the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();
	
	while(1)
	{
		if(Paramter->Chr_arr[Paramter->Char_Num-1] == ';')
		{
			LCD_clearScreen();
			Paramter->Char_Num = 0;
			Paramter->Tmp = 0;	
		}
		else
		{
			if((Paramter->Tmp) < (Paramter->Char_Num) )
			{
				for(Paramter->Tmp ; (Paramter->Tmp) < (Paramter->Char_Num) ; (Paramter->Tmp++))
				{
					if(Paramter->Tmp < (MAX_CHAR_NUM - 1)){
						LCD_displayCharacter(Paramter->Chr_arr[Paramter->Tmp]);
						
						if(Paramter->Chr_arr[Paramter->Tmp] == Paramter->Pass[Paramter->Tmp])
						{
							Cmp_Res++;
						}
					}
				}
			}
			else if(Paramter->Tmp == MAX_CHAR_NUM && Paramter->Chr_arr[MAX_CHAR_NUM - 1] == ':')
			{
				if(Cmp_Res == PASS_MATCHED)
				{
					LCD_clearScreen();
					
					LCD_displayStringRowColumn(0,0," CorrectPassword ");
					Paramter->Tmp++;
					vTaskDelay(DELAY_2_SECONDS);
					LCD_clearScreen();
				}
				else
				{
					LCD_clearScreen();
					
					LCD_displayStringRowColumn(0,0," WRONG ONE    ");
					vTaskDelay(DELAY_2_SECONDS);
					LCD_clearScreen();
					Paramter->Tmp++;
				}
			}	
		}

		vTaskDelayUntil(&xLastWakeTime , xFrequency);
	}
}


int main(void)
{
	
	/*Create Tasks Handles*/
	TaskHandle_t Sys_Init = ZERO;
	TaskHandle_t KeyPad_Task = ZERO;
	TaskHandle_t LCD_Task = ZERO;
	
	/*Task Creation*/
	xTaskCreate(LCD_Write,"LCD" , STACK_100_WORD , &str_Task ,PERIORITY_1, &LCD_Task);
	xTaskCreate(KeyPad,"KeyPad" , STACK_100_WORD , &str_Task ,PERIORITY_2, &KeyPad_Task);
	xTaskCreate(LCD_Init,"SysIn" , STACK_100_WORD , (void * ) NO_PARAMETERS , PERIORITY_3 , &Sys_Init);
	
	/* Start Scheduler */
	vTaskStartScheduler();

	while(1)
	{
	
	}
}
