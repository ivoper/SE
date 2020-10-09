/*
 * app_mem_managment.h
 *
 *  Created on: 18/12/2018
 *      Author: A42172
 */

#ifndef APP_MEM_MANAGMENT_H_
#define APP_MEM_MANAGMENT_H_

#include <time.h>

//sizes
#define CHUNK_SIZE		0x0100	//256 bytes
#define INT_SIZE		4
#define SECTOR_SIZE		0x8000
#define MAINTENANCE_ACCESS_DATA_STRUCT_SIZE	0x1000
#define LOCK_ACCESS_DATA_STRUCT_SIZE		0x7000

//sector addresses
#define SECTOR_27_ADDRESS		0x00068000
#define SECTOR_28_ADDRESS		0x00070000
#define SECTOR_29_ADDRESS		0x00078000

//data structures addresses
#define CONTROL_ADDRESS				SECTOR_27_ADDRESS
#define RECORDS_ADDRESS				(struct chunk *)SECTOR_28_ADDRESS
#define	LOCK_ACCESS_ADDRESS			(struct chunk *)SECTOR_29_ADDRESS
#define MAINTENANCE_ACCESS_ADDRESS	(struct chunk *)(SECTOR_29_ADDRESS+LOCK_ACCESS_DATA_STRUCT_SIZE)

//indexes of lengths in CONTROL data structure
#define RECORDS_POSITION		0x00
#define LOCK_POSITION			0x01
#define MAINTENANCE_POSITION	0x02

//addresses of lengths in CONTROL
#define RECORDS_LENGTH_ADDRESS		(unsigned int *)(CONTROL_ADDRESS + (CHUNK_SIZE * RECORDS_POSITION))
#define LOCK_LENGTH_ADDRESS			(unsigned int *)(CONTROL_ADDRESS + (CHUNK_SIZE * LOCK_POSITION))
#define MAINTENANCE_LENGTH_ADDRESS	(unsigned int *)(CONTROL_ADDRESS + (CHUNK_SIZE * MAINTENANCE_POSITION))

struct record{
	unsigned int cardId;
	struct tm date;
	char valid;
};

struct chunk{
	char byte[CHUNK_SIZE];
};

void HardResetFlash();

void AddRecord(struct record record);
void AddLockCard(unsigned int id);
void AddMaintenanceCard(unsigned int id);

#endif /* APP_MEM_MANAGMENT_H_ */
