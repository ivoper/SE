/**
* @file		flash.h
* @brief	Contains Flash memory handling functions.
* @version	1.2
* @date		21 Dec 2018
* @author	Grupo 4
*/

#ifndef FLASH_H_
#define FLASH_H_

/** @defgroup FLASH FLASH
 * This package provides the functions to use Flash memory.
 * @{
 */

/** @defgroup FLASH_Public_Functions FLASH Public Functions
 * @{
 */

//the values returned by these functions correspond to the IAP status codes [D1].

/**
 * @brief	Erases the content of Flash sectors from startSector to endSector, returning the status code.
 * @param 	startSector: initial sector to erase.
 * @param 	endSector: last sector to erase.
 * @return 	unsigned int.
 */
unsigned int FLASH_EraseSectors(unsigned int startSector, unsigned int endSector);

/**
 * @brief	Writes a data block to the Flash memory, returning the status code.
 * @param 	dstAddr: reference to the destination address.
 * @param 	srcAddr: reference to the source address.
 * @param 	size: number of bytes to write.
 * @return 	unsigned int.
 */
unsigned int FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size);

/**
 * @brief	Compares a Flash memory data block, returning the status code.
 * @param 	dstAddr: reference to the destination address.
 * @param 	srcAddr: reference to the source address.
 * @param 	size: number of bytes to compare.
 * @return 	unsigned int.
 */
unsigned int FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size);

/**
 * @}
 */


/**
 * @}
 */

#endif /* FLASH_H_ */
