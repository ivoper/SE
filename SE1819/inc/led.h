/**
* @file		led.h
* @brief	Contains the LED API.
* @version	1.0
* @date		16 Out 2018
* @author	Grupo 4
*/

#ifndef LED_H_
#define LED_H_

/** @defgroup LED LED
 * This package provides the core capabilities such as on / off for the use of a LED
 * connected to or in the LPC1769 from NXP.
 * @{
 */

/** @defgroup LED_Public_Functions LED Public Functions
 * @{
 */

/**
 * @brief	Initializes the LED API
 * @param	portId	: GPIO port associated to the LED.
 * @param	pinId	: Index of pin in which the LED is located.
 * @param	state	: State of the LED. The 0 indicate led off and 1 LED on.
 * @return	Void
 * @note	This function must be called prior to any other LED functions. The
 * LED will start in the value passed in the parameter.
 */
void LED_Init(int portId, int pinId, int state);

/**
 * @brief	Get LED state.
 * @return	status of LED. The 0 indicate led is off and 1 LED is on.
 */
int LED_GetState(void);

/**
 * @brief	Turn LED on
 * @return	Nothing
 */
void LED_On(void);

/**
 * @brief	Turn LED off
 * @return	Nothing
 */
void LED_Off(void);
/**
 * @}
 */


/**
 * @}
 */

#endif /* LED_H_ */
