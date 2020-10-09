/**
* @file		i2c.h
* @brief	Contains I2C API.
* @version	1.1
* @date		26 Fev 2019
* @author	Ivo Pereira
*/

#ifndef I2C_H_
#define I2C_H_

#include <stdbool.h>

/** @defgroup I2C I2C
 * This package provides the core capabilities for an I2C communication.
 * @{
 */

/** @defgroup I2C_Public_Functions I2C Public Functions
 * @{
 */

#define I2C_PORT 2		//using port 2 of the three available(0/1/2)

#define SUCCESSFUL_STOPED	1
#define SUCCESSFUL_HALTED	0
#define UNSUCCESSFUL		-1

/**
 * @brief	Initializes the I2C controller, configures the respective pins and sets the bit frequency.
 * @param clh: count for SCL high time period.
 * @param cll: count for SCL low time period.
 * @return	Void.
 */
void I2C_Init(short clh, short cll);

/**
 * @brief	Using the I2C protocol transfers data to a slave. Returns positive if
 * 			successful or 0 if not successful.
 * @param 	SLA: Slave address.
 * @param 	length: Number of bytes to be transfered.
 * @param 	data: Pointer to array of bytes to be transfered.
 * @param	toStop: Determines if a stop condition is to be sent in the end of the transfer.
 * @return 	int.
 */
int I2C_Transfer(unsigned char SLA, unsigned int length, unsigned char* data, bool toStop);

/**
 * @brief	Using the I2C protocol receives data from a slave. Returns positive if
 * 			successful or 0 if not successful.
 * @param 	SLA: Slave address.
 * @param 	length: Number of bytes to be received.
 * @param 	data: Pointer to memory position where received bytes will be placed.
 * @return 	int.
 */
int I2C_Receive(unsigned char SLA, unsigned int length, unsigned char* data);

/**
 * @}
 */


/**
 * @}
 */

#endif /* I2C_H_ */
