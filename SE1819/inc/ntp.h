/**
* @file		ntp.h
* @brief	Contains the API to the NTP protocol.
* @version	1.0
* @date		02 Jul 2019
* @author	Ivo Pereira
*/

#ifndef NTP_H_
#define NTP_H_

/** @defgroup NTP NTP
 * This package allows the request of the current time using NTP through an internet connection.
 * @{
 */

/** @defgroup NTP_Public_Functions NTP Public Functions
 * @{
 */

/**
 * @brief	Returns the current time in the format of a timestamp of seconds since 1900.
 * @return	unsigned long.
 */
unsigned long NTP_GetTime();

/**
 * @}
 */


/**
 * @}
 */

#endif /* NTP_H_ */
