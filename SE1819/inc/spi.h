/**
* @file		spi.h
* @brief	Contains SPI API.
* @version	1.2
* @date		21 Dec 2018
* @author	Grupo 4
*/

#ifndef SPI_H_
#define SPI_H_

/** @defgroup SPI SPI
 * This package provides the core capabilities for the SPI.
 * @{
 */

/** @defgroup SPI_Public_Functions SPI Public Functions
 * @{
 */

/**
 * @brief	Initializes the SPI controller and configures the respective pins.
 * @return	Void.
 */
void SPI_Init(void);

/**
 * @brief	Configures the SPI transmit frequency, number of data bits and mode.
 * @param 	frequency: the frequency on which the spi will send or recieve data.
 * @param 	bitData: number of data bits.
 * @param 	mode: mode in which the spi will operate (CPOL, CPHA).
 * @return 	Void.
 */
void SPI_ConfigTransfer(int frequency, int bitData, int mode);

/**
 * @brief	Sets the slave's chip select to active.
 * @param 	csBitId: slave's cs bit.
 * @return 	Void.
 */
void SPI_BeginTransfer(int csBitId);

/**
 * @brief	Sets the slave's chip select to inactive.
 * @param 	csBitId: slave's cs bit.
 * @return 	Void.
 */
void SPI_EndTransfer(int csBitId);

/**
 * @brief	Performs a transfer.
 * @param 	txBuffer: buffer to transmit data.
 * @param 	rxBuffer: buffer to receive data.
 * @param 	length: number of data chunks to be transfered.
 * @return 	Void.
 */
void SPI_Transfer(unsigned short *txBuffer, unsigned short *rxBuffer, int
length);

/**
 * @}
 */


/**
 * @}
 */

#endif /* SPI_H_ */
