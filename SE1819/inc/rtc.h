/**
* @file		rtc.h
* @brief	Contains RTC API.
* @version	1.2
* @date		21 Dec 2018
* @author	Grupo 4
*/

#ifndef RTC_H_
#define RTC_H_

#include <time.h>

/** @defgroup RTC RTC
 * This package provides the core capabilities for the real time clock.
 * @{
 */

/** @defgroup RTC_Public_Functions RTC Public Functions
 * @{
 */

/**
 * @brief	Initialises the system to allow for RTC periferal access.
 * @param 	seconds: Clock initialized with this value that represents seconds since 00:00:00 UTC January 1, 1970.
 * @return	Void.
 */
void RTC_Init(time_t seconds);

/**
 * @brief	Returns the current value of RTC through the dateTime parameter.
 * @param 	dateTime: (out) Variable that will have the RTC value placed in.
 * @return 	Void.
 */
void RTC_GetValue(struct tm *dateTime);

/**
 * @brief	Sets the RTC value with the dateTime parameter.
 * @param 	dateTime: Time struct that has the values to set the RTC.
 * @return 	Void.
 */
void RTC_SetValue(struct tm *dateTime);

/**
 * @brief	Sets the RTC value with the seconds parameter.
 * @param 	seconds: Value that will be placed in RTC. Represents seconds since 00:00:00 UTC January 1, 1970.
 * @return 	Void.
 */
void RTC_SetSeconds(time_t seconds);

/**
 * @brief	Returns the current value of RTC in seconds since 00:00:00 UTC January, 1970.
 * @return 	time_t.
 */
time_t RTC_GetSeconds(void);

/**
 * @}
 */


/**
 * @}
 */

#endif /* RTC_H_ */
