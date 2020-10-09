/**
* @file		e2prom.h
* @brief	Contains EEPROM API.
* @version	1.0
* @date		19 Mar 2019
* @author	Ivo Pereira
*/

#ifndef E2PROM_H_
#define E2PROM_H_

/** @defgroup E2PROM E2PROM
 * This package provides the core capabilities to use an EEPROM as storage.
 * @{
 */

/** @defgroup E2PROM_Public_Functions E2PROM Public Functions
 * @{
 */

#define UNSUPPORTED_BYTE_NUMBER -2

/**
 * @brief	Initializes the I2C controller with a supported frequency, to be used as communication with the EEPROM.
 * @return	Void.
 */
void E2P_Init();

/**
 * @brief	Writes data in the EEPROM. Returns positive if
 * 			successful or 0 if not successful.
 * @param 	address: Address in EEPROM where data is to be written.
 * @param 	n_bytes: Number of bytes to be written.
 * @param 	buffer: Pointer to array of bytes to be transfered.
 * @return 	int.
 */
int E2P_Write(unsigned short address, unsigned int n_bytes, unsigned char *buffer);

/**
 * @brief	Reads data from the EEPROM. Returns positive if
 * 			successful or 0 if not successful.
 * @param 	address: Address in EEPROM where data is to be read.
 * @param 	n_bytes: Number of bytes to be read.
 * @param 	buffer: Pointer to memory position to write the recieved bytes.
 * @return 	int.
 */
int E2P_Read(unsigned short address, unsigned int n_bytes, unsigned char *buffer);

/**
 * @}
 */


/**
 * @}
 */

#endif /* E2PROM_H_ */
