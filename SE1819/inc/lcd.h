/**
* @file		lcd.h
* @brief	Contains the LCD API.
* @version	1.0
* @date		16 Out 2018
* @author	Grupo 4
*/

#ifndef LCD_H_
#define LCD_H_

/** @defgroup LCDText LCDText
 * This package provides the core capabilities to write for a 16*2 display
 * connected to the LPC1769 from NXP.
 * @{
 */

/** @defgroup LCDText_Public_Functions LCDText Public Functions
 * @{
 */

/**
 * @brief Initializes the system to allow access to the peripheral using 2 lines, 16 columns and 4bit communication.
 * @return Void
 */
void LCDText_Init(void);

/**
 * @brief Writes a character in the actual cursor position.
 * @param ch: character to write
 * @return Void
 */
void LCDText_WriteChar(char ch);

/**
 * @brief Writes a string in the actual cursor position.
 * @param str: string to write
 * @return Void
 */
void LCDText_WriteString(char *str);

/**
 * @brief Positions the cursor in the line row and column column.
 * @param row: row selected
 * @param column: column selected
 * @return Void
 */
void LCDText_Locate(unsigned int row, unsigned int column);

/**
 * @brief Clears the display.
 * @return Void
 */
void LCDText_Clear(void);

/**
 * @brief Writes the formated string fmt to the display.
 * @param fmt: formated string
 * @param ...: arguments for the format
 * @return Void
 */
void LCDText_Printf(char *fmt, ...);
/**
 * @}
 */


/**
 * @}
 */

#endif /* LCD_H_ */
