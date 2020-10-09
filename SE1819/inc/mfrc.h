/**
* @file		mfrc.h
* @brief	Contains MFRC low level functions.
* @version	1.2
* @date		21 Dec 2018
* @author	Grupo 4
*/

#ifndef MFRC_H_
#define MFRC_H_

#include <stdbool.h>
#include "LPC17xx.h"

/** @defgroup MFRC MFRC
 * This package provides some "low level"(close to hardware) functions for the mfrc522 api to use.
 * @{
 */

/** @defgroup MFRC_Public_Functions MFRC Public Functions
 * @{
 */

/**
 * @brief	Runs necessary initializations for the library.
 * @return	Void.
 */
void MFRC522_PreInit(void);

/**
 * @brief	Handles the reset pin of RC522 according to the assert parameter.
 * @param 	assert: boolean that defines if reset is to be set.
 * @return 	Void.
 */
void MFRC522_Reset(bool assert);

/**
 * @brief	Handles the chip select pin of RC522 according to the assert parameter.
 * @param 	assert: boolean that defines if CS is to be set.
 * @return 	Void.
 */
void MFRC522_Select(bool assert);

/**
 * @brief	Performs the transmission/reception of a byte returning the received byte.
 * @param 	value: byte to be transfered.
 * @return 	uint8_t.
 */
uint8_t MFRC522_Transfer(uint8_t value);

/**
 * @brief	Waits the value of the millis param in milliseconds.
 * @param 	millis: number of milliseconds to wait.
 * @return 	Void.
 */
void MFRC522_WaitMs(uint32_t millis);

/**
 * @}
 */


/**
 * @}
 */

#endif /* MFRC_H_ */
