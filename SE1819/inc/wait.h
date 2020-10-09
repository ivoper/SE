/**
* @file		wait.h
* @brief	Contains the delay API.
* @version	1.2
* @date		08 Dec 2018
* @author	PSampaio, Grupo 4
*/

#ifndef WAIT_H_
#define WAIT_H_

/** @defgroup WAIT Wait
 * This package provides the core capabilities for wait functions.
 * @{
 */

/** @defgroup WAIT_Public_Functions Wait Public Functions
 * @{
 */

/**
 * @brief	Initialises the wait API for 1 ms resolution.
 * @return	0 if initialisation successed; 1 if fails.
 * @note	This function must be called prior to any other WAIT functions.
 */
int WAIT_Init(void);


/**
 * @brief	Waits a number of milliseconds.
 * @param	millis: the whole number of milliseconds to wait.
 */
void WAIT_Ms(unsigned int millis);

/**
 * @}
 */


/**
 * @}
 */

#endif /* WAIT_H_ */
