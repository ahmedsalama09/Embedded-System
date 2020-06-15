/*
 * bcm_cfg.h
 *
 * Created: 2020-03-01 10:17:16 PM
 *  Author: EbrahimOseif
 */ 


#ifndef BCM_CFG_H_
#define BCM_CFG_H_

/************************************************************************/
/*								DEFINES                                 */
/************************************************************************/
#define MAX_BUFFER_SIZE   (1000)

/* TODO: Add support for SPI and I2C */
/* UART configuration parameters */
#define UART_BAUD_RATE    (9600)

/* UART module states */
#define TX_ONLY           (0)
#define RX_ONLY           (1)
#define TX_RX             (2)

/* Configure UART module state to be used to initialize UART module */
#define UART_TX_RX_CONFIG TX_RX


/*BCM Struct






#endif /* BCM_CFG_H_ */