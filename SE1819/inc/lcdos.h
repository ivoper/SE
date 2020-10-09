/**
* @file		lcdos.h
* @brief	Contains the Display/lcd API to use with FreeRTOS.
* @version	1.0
* @date		06 Jun 2018
* @author	Ivo Pereira
*/

#ifndef LCDOS_H_
#define LCDOS_H_

#include "FreeRTOS.h"
#include "queue.h"

/** @defgroup LCDOS LCDos
 * This package provides the core capabilities to write for a 16*2 display
 * connected to the LPC1769 from NXP.To be used with FreeRTOS.
 * Supports multiple tasks, where one communicates with the display controller
 * and others that queue commands.
 * @{
 */

/** @defgroup LCDOS_Public_Functions LCDos Public Functions
 * @{
 */

#define ILLEGAL_ROW 	-2
#define ILLEGAL_COL		-3
#define CMD_SIZE		33

/**
 * @brief Initializes the system to allow access to the peripheral using 2 lines, 16 columns and 4bit communication.
 * @param queue: queue object that will hold commands to be send by the lcd task to the display.
 * @return Void
 */
void LCDOS_Init(QueueHandle_t queue);

/**
 * @brief Waits for queued commands and sends them to the display.
 * @return void
 */
void LCDOS_ProcessCmd();

/**
 * @brief Queues command that positions the cursor in the line row and column column.
 * 		  Returns 1 if successfully queued and 0 otherwise.
 * @param row: row selected
 * @param column: column selected
 * @return int
 */
int LCDOS_Locate(unsigned int row, unsigned int column);

/**
 * @brief Queues command that clears the display. Returns 1 if successfully queued and 0 otherwise.
 * @return int
 */
int LCDOS_Clear(void);

/**
 * @brief Queues command that writes the formated string fmt to the display.
 * 		  Returns 1 if successfully queued and 0 otherwise.
 * @param fmt: formated string
 * @param ...: arguments for the format
 * @return Void
 */
int LCDOS_Printf(char *fmt, ...);

/**
 * @}
 */


/**
 * @}
 */

#endif /* LCDOS_H_ */
