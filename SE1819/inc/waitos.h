/**
* @file		waitos.h
* @brief	Contains the delay API to use with FreeRTOS.
* @version	1.0
* @date		04 Jun 2019
* @author	Ivo Pereira
*/

#ifndef WAITOS_H_
#define WAITOS_H_

/** @defgroup WAITOS Waitos
 * This package provides the core capabilities for wait functions to use with FreeRTOS.
 * @{
 */

/** @defgroup WAITOS_Public_Functions Waitos Public Functions
 * @{
 */

/**
 * @brief	Waits a number of milliseconds.
 * @param	millis: the whole number of milliseconds to wait.
 */
void WAITOS_Ms(unsigned int millis);

/**
 * @}
 */


/**
 * @}
 */

#endif /* WAITOS_H_ */
