/*
 * SPI.h
 *
 * Created: 10-Feb-20 6:04:17 PM
 *  Author: ahmed
 */ 


#ifndef SPI_H_
#define SPI_H_
#include "/tmu/BCM-1/BCM-1/std_types.h"
#include "SPI_CONFIG.h"
#include "registers.h"



ERROR_STATUS spi_init(config_spi *sp_inits);
ERROR_STATUS spi_transeiver(uint8_t send_spi);
ERROR_STATUS spi_receiver(uint8_t *u8_spi_reception);

#endif /* SPI_H_ */