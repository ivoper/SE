/**
* @file		wifi.h
* @brief	Allows communication to a module that provides Wifi.
* @version	1.1
* @date		25 Apr 2019
* @author	Ivo Pereira
*/

#ifndef WIFI_H_
#define WIFI_H_

#include <stdbool.h>

/** @defgroup WIFI Wifi
 * This package allows communication to a module that provides Wifi.
 * @{
 */

/** @defgroup WIFI_Public_Functions Wifi Public Functions
 * @{
 */

/**
 * @brief	Initializes the UART controller with a supported frequency, to be the communication protocol used,
 * 			and configures the pins used.
 * @return	Void.
 */
void WIFI_Init();

/**
 * @brief	Sends a command and waits for the response. Returns true if
 * 			successful or false if not successful.
 * @param 	cmd: Pointer to array of bytes that make up the command to send.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @param	waitOK: Determines if an "OK" is expected at the end of the response.
 * @return 	bool.
 */
bool WIFI_Cmd(unsigned char* cmd, unsigned char* response, unsigned int *length, bool waitOK);

/**
 * @brief	Sends data and waits for the response. Returns true if
 * 			successful or false if not successful.
 * @param 	data: Pointer to array of bytes that make up the data to send.
 * @param	size: Amount of bytes of data to send.
 * @param	response: Pointer to position in memory where the response is going to be written.
 * @param 	length: Pointer to a variable where the length of the response is going to be written.
 * @param	waitOK: Determines if an "OK" is expected at the end of the response.
 * @return 	bool.
 */
bool WIFI_Data(unsigned char* data, unsigned int size, unsigned char* response, unsigned int *length, bool waitOK);

/**
 * @}
 */


/**
 * @}
 */

#endif /* WIFI_H_ */
