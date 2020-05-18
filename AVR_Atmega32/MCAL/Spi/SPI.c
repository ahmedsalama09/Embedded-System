/*
 * SPI.c
 *
 * Created: 10-Feb-20 6:11:46 PM
 *  Author: ahmed
 */
#include "SPI.h"
#include "SPI_CONFIG.h"
volatile uint8_t spi_myflag =0;
volatile uint8_t data=0;

ERROR_STATUS spi_init(config_spi *sp_inits){//uint8_t spi_irq,uint8_t spi_en,uint8_t bit_first,uint8_t master_slave,uint8_t smp,uint8_t spi_prescal){
	
	/*create error flag*/
	ERROR_STATUS error_flag = E_OK;
	
	SPCR = ((sp_inits->spi_irq)  | (sp_inits->bit_first) | (sp_inits->smp) | (sp_inits->spi_prescal) | (sp_inits->master_slave));
	
	//if((sp_inits->master_slave) == MASTER_SELECT)
	switch(sp_inits->master_slave)
	{
		case MASTER_SELECT:
		/*set MOSI as OUT*/

			PORTB_DIR |= (MOSI);
			PORTB_DIR &= ~(MISO);
		
			PORTB_DIR |= (SS);
			/*set ck as out*/
			PORTB_DIR |= (SCK);
		
		break;
		
		case SLAVE_SELECT:
		
			/*set MOSI as IN*/
			PORTB_DIR &= ~(MOSI);
			PORTB_DIR |= (MISO);
		
			PORTB_DIR &= ~(SS);
			/*set ck as in*/
			PORTB_DIR &= ~(SCK);
		
		break;
			
			/*save error status*/
			error_flag = E_NOK;
		default:
			

		break;
	}
	/*enable spi*/
	SPCR |=  (sp_inits->spi_en);
	
	/*return error status*/
	return error_flag;
}

ERROR_STATUS spi_transeiver(uint8_t send_spi){
		
	if(SPSR & SPIF_FLAG)
	{
		data = SPDR ;
	}
	else
	{
		/*send data*/
		SPDR = send_spi;
	}
	return E_OK;
}

ERROR_STATUS spi_receiver(uint8_t *u8_spi_reception)
{
	/*create error flag*/
	ERROR_STATUS error_flag = E_NOK;
	
	if(SPSR & SPIF_FLAG)
	{
		*u8_spi_reception = SPDR;
		error_flag = E_OK;
	}
	
	return error_flag;
}

/*if((SPSR & SPIF_FLAG) && write_me == YOU_CAN_NOT)
{
	dummy = SPDR;
	
	write_me = YOU_CAN_NOW;

}
else if(write_me == YOU_CAN_NOW)
{
	SPDR = 'a';
	
	write_me = YOU_CAN_NOT;
}*/