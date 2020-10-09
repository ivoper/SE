/**
* @file		keypados.h
* @brief	Keypad API to use with FreeRTOS.
* @version	1.0
* @date		04 Jun 2019
* @author	Ivo Pereira
*
*/

#ifndef KEYPADOS_H_
#define KEYPADOS_H_

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/** @defgroup KEYPADOS Keypados
 * This package provides the core capabilities for a keypad
 * connected to the LPC1769 from NXP. To be used with FreeRTOS.
 * Supports multiple tasks, where one processes the keys pressed
 * and others can get the keys that were pressed.
 * @{
 */

/** @defgroup KEYPADOS_Public_Functions Keypados Public Functions
 * @{
 */

/**
 * Number of lines and columns in the keypad
 */
#define NUMBER_OF_LINES 4
#define NUMBER_OF_COLUMNS 3

/**
 * This keypad does NOT support any key combination (keys pressed at the same time) of two keys
 * in DIFFERENT LINES, the key pressed in the uppermost row will PROBABLLY be the one recorded (not guaranteed).
 *
 * This keypad only supports a single key combination that returns COMBINATION_CODE
 * all other combinations tried in the same line will return KEY_COMBINATION_UNSUPPORTED_CODE
 */
#define KEY_COMBINATION_UNSUPPORTED_CODE	-1
#define COMBINATION_CODE 	127

/**
 * @brief Initializes the system to allow access to the peripheral. As such,
 * 		  receives in the parameters the vector with the codes of the 12 keys (layout),
 * 		  and the OS objects that will be used to communicate between tasks.
 * @param layout: array with the keys' codes
 * @param queue: queue object that holds the keys pressed so different tasks can fetch
 * @param smphr: semaphore object that wakes thread that will process keys when a key is pressed
 * @return Void
 */
void KEYPADOS_Init(int layout[12], QueueHandle_t queue, SemaphoreHandle_t smphr);

/**
 * @brief Waits for a key press, processes the key pressed and pushes it to the queue.
 * 		  to be used by the keypad task.
 * @return void
 */
void KEYPADOS_ProcessKey();

/**
 * @brief Pools keypad to determine which key was pressed and returns that key code.
 * @return int
 */
int KEYPADOS_PoolKeypad();

/**
 * @brief Returns the first key in the queue.
 * @param ticks: number of ticks to wait for a key.
 * @return int
 */
int KEYPADOS_GetKey(TickType_t ticks);

/**
 * @}
 */


/**
 * @}
 */

#endif /* KEYPADOS_H_ */
