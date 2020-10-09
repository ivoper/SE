/**
* @file		uart.h
* @brief	Contains the UART API.
* @version	1.0
* @date		09 Apr 2019
* @author	Ivo Pereira
*/

#ifndef UART_H_
#define UART_H_

#include <stdbool.h>

/** @defgroup UART UART
 * This package provides the core capabilities for an UART communication.
 * Has minor optimizations to use with esp module.
 * @{
 */

/** @defgroup UART_Public_Functions UART Public Functions
 * @{
 */

#define UART_PORT		0

/**
 * @brief	Resets the UART transmit and receive buffers.
 * @return 	void.
 */
void ResetBuffers();

/**
 * @brief	Initializes the UART controller, configures the respective pins and sets the baudrate.
 * @param baudrate: baudrate to be defined.
 * @return	Void.
 */
void UART_Init(unsigned int baudrate);

/**
 * @brief	Sends data using UART. Returns true if
 * 			successful or false if not successful.
 * @param 	buffer: Pointer to array of bytes to be transfered.
 * @param 	size: Number of bytes to be transfered.
 * @return 	bool.
 */
bool UART_Send(unsigned char *buffer, unsigned int size);

/**
 * @brief	Receives data through UART. Returns true if
 * 			successful or false if not successful.
 * @param 	buffer: Pointer to position of memory where received bytes will be stored.
 * @param 	size: Number of bytes to receive.
 * @param	waitOK: Determines if an "OK" is expected at the end of the packet.
 * @return 	bool.
 */
bool UART_Recieve(unsigned char *buffer, unsigned int size, bool waitOK);

/**
 * @}
 */


/**
 * @}
 */

#endif /* UART_H_ */
