/**
* @file		keypad.h
* @brief	KEYPAD API.
* @version	1.0
* @date		16 Out. 2018
* @author	Grupo 4
*
*/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/** @defgroup KEYPAD KEYPAD
 * This package provides the core capabilities for a keypad
 * connected to the LPC1769 from NXP.
 * @{
 */

/** @defgroup KEYPAD_Public_Functions KEYPAD Public Functions
 * @{
 */

/**
 * Number of lines and columns in the keypad
 */
#define NUMBER_OF_LINES 4
#define NUMBER_OF_COLUMNS 3

/**
 * This keypad does NOT support any key combination (pressed at the same time) of two keys
 * in DIFFERENT LINES, application will block until one of the keys is released.
 *
 * This keypad only supports a single key combination that returns COMBINATION_CODE
 * all other combinations tried in the same line will return KEY_COMBINATION_UNSUPPORTED_CODE
 */
#define KEY_COMBINATION_UNSUPPORTED_CODE	-1
#define COMBINATION_CODE 	256

/**
 * @brief Initializes the system to allow access to the peripheral. As such,
 * 		  receives in the parameters the vector with the codes of the 12 keys (layout)
 * @param layout: array with the keys' codes
 * @return Void
 */
void KEYPAD_Init(int layout[12]);

/**
 * @brief Returns 1 if a key is being pressed or 0 otherwise. Non-blocking operation
 * @return int
 */
int KEYPAD_Hit(void);

/**
 * @brief Returns the code of the pressed key. Blocking operation
 * @return int
 */
int KEYPAD_Read(void);

/**
 * @}
 */


/**
 * @}
 */

#endif /* KEYPAD_H_ */
