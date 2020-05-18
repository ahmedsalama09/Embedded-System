/*
 * spi_confg.c
 *
 * Created: 02-Mar-20 7:27:36 AM
 *  Author: ahmed
 */ 

/************************************************************************/
/*								INCLUDES                                */
/************************************************************************/
#include "SPI_CONFIG.h"


/*write all spi inits here*/
config_spi st_spi_inits = {SPI_INT_DIS , SPI_EN , SPI_LSB_FIRST , MASTER_SELECT , R_SAMPLE_R_SETUP , SPI_PRE_16};