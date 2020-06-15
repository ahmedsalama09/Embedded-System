#include "FreeRTOS.h"
#include "task.h"
#include "/LCD_APP/MCAL/DIO.h"
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



/************************************************************************/
/*								APIS                                    */
/************************************************************************/
/*Create Dio Struct*/
st_DIO_Cfg_t st_Dio_init;

/*LCDInit
*Description : Task To init LCD once then Suspended
..It has the highest priority
*/
void System_Init(void * pvParameters)
{
	while(1)
	{
	st_Dio_init.GPIO = GPIOA;
	st_Dio_init.pins = LOWER_NIBBLE;
	st_Dio_init.dir = OUTPUT;
	
	DIO_init(&st_Dio_init);
	st_Dio_init.GPIO = GPIOA;
	st_Dio_init.pins = PIN4;
	st_Dio_init.dir = INPUT;
	
	DIO_init(&st_Dio_init);
	
	DIO_Write(GPIOA , PIN0 , HIGH);
	

	vTaskSuspend(NULL);
	}
}

void Traffic_Task(void *pvParameters)
{
	uint8_t Btn_Flag = 0,Timing_Cnt = 0 , data =0;
	
	while(1)
	{
		DIO_Read(GPIOA,PIN4,&data);
		if(data != 0)
		{
			Btn_Flag = 1;
		}
		else if(data == 0 && Btn_Flag ==1)
		{
			Timing_Cnt++;
			if(Timing_Cnt == 1)
			{
				DIO_Write(GPIOA,PIN1,HIGH);
				DIO_Write(GPIOA , PIN0 , LOW);
			}
			
			
			else if(Timing_Cnt >= 16 && Timing_Cnt < 21)
			{
				DIO_Toggle(GPIOA , PIN1);
			}
			
			else if(Timing_Cnt > 21 && Timing_Cnt <= 56)
			{
				DIO_Write(GPIOA , PIN1 , LOW);
				DIO_Write(GPIOA , PIN2 , HIGH);
			}
			else if(Timing_Cnt > 56 && Timing_Cnt < 71)
			{
				DIO_Toggle(GPIOA , PIN2);
			}
			else if (Timing_Cnt > 71)
			{
				Timing_Cnt =0;
				Btn_Flag =0;
				DIO_Write(GPIOA,PIN2,LOW);
				DIO_Write(GPIOA,PIN0,HIGH);
			}
			
		}
		
		vTaskDelay(200);
	}
}

int main(void)
{
	
	/*Create Tasks Handles*/
	TaskHandle_t Sys_Init = ZERO;
	TaskHandle_t Traffic_Light = ZERO;
	
	/*Task Creation*/
	xTaskCreate(Traffic_Task,"Traffic" , STACK_100_WORD , (void *)NO_PARAMETERS ,PERIORITY_2, &Traffic_Light);
	xTaskCreate(System_Init,"SysIn" , 300 , (void * ) NO_PARAMETERS , PERIORITY_3 , &Sys_Init);
	
	/* Start Scheduler */
	vTaskStartScheduler();

	while(1)
	{
	
	}
}
