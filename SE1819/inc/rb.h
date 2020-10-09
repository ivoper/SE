/**
* @file		rb.h
* @brief	Contains an API to use a ring buffer.
* @version	1.1
* @date		16 Apr 2019
* @author	Ivo Pereira
*/

#ifndef RB_H_
#define RB_H_

#include <stdbool.h>

/** @defgroup RB RB
 * This package provides a functional API to manipulate a ring buffer.
 * @{
 */

/** @defgroup RB_Public_Functions RB Public Functions
 * @{
 */

#define RB_BUFFER_SIZE				256

//unpredictable behavior if pushed full or poped empty, use functions to check for empy or full states

typedef struct ringbuffer{
	unsigned char buffer[RB_BUFFER_SIZE];
	unsigned char put;
	unsigned char get;
	bool full;
}RINGBUFF_DSC;

/**
 * @brief	Resets the buffer.
 * @param 	buffer: the buffer to be reset.
 * @return 	void.
 */
void RB_Reset(RINGBUFF_DSC *buffer);

/**
 * @brief	Returns true if the buffer is full and false otherwise.
 * @param 	buffer: the buffer to be checked.
 * @return 	bool.
 */
bool RB_Full(RINGBUFF_DSC *buffer);

/**
 * @brief	Returns true if the buffer is empty and false otherwise.
 * @param 	buffer: the buffer to be checked.
 * @return 	bool.
 */
bool RB_Empty(RINGBUFF_DSC *buffer);

/**
 * @brief	Pushes a value into a buffer.
 * @param 	buffer: the buffer to push the value.
 * @param	val: the value to be pushed
 * @return 	void.
 */
void RB_Push(RINGBUFF_DSC *buffer, unsigned char val);

/**
 * @brief	Pops a value from a buffer and returns that value.
 * @param 	buffer: the buffer to pop the value.
 * @return 	unsigned char.
 */
unsigned char RB_Pop(RINGBUFF_DSC *buffer);

/**
 * @}
 */


/**
 * @}
 */

#endif /* RB_H_ */
