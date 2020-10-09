/*
 * app_mem_managment.c
 *
 *  Created on: 18/12/2018
 *      Author: A42172
 */

#include "app_mem_managment.h"
#include "flash.h"
#include <string.h>

#define MASTER_CARD_ID 0x1F016D86

//lengths in control set to 0
void SetUpControl(){
	char array[INT_SIZE] = {0, 0, 0, 0};	//4 chars = 1 integer
	unsigned int *pointer = RECORDS_LENGTH_ADDRESS;
	FLASH_WriteData(pointer, array, INT_SIZE);
	pointer = LOCK_LENGTH_ADDRESS;
	FLASH_WriteData(pointer, array, INT_SIZE);
	pointer = MAINTENANCE_LENGTH_ADDRESS;
	FLASH_WriteData(pointer, array, INT_SIZE);
}
//default maintenance card
void SetUpDefaultCard(){
	AddMaintenanceCard(MASTER_CARD_ID);
}


//0-records, 1-lock, 2-maintenance "generic"
void IncrementLength(unsigned int pos){
	//backup
	unsigned int record;
	unsigned int lock;
	unsigned int maint;
	memcpy(&record, RECORDS_LENGTH_ADDRESS, INT_SIZE);
	memcpy(&lock, LOCK_LENGTH_ADDRESS, INT_SIZE);
	memcpy(&maint, MAINTENANCE_LENGTH_ADDRESS, INT_SIZE);
	FLASH_EraseSectors(27, 27);		//erase
	switch(pos){					//increment
		case RECORDS_POSITION:
			++record;
			break;
		case LOCK_POSITION:
			++lock;
			break;
		case MAINTENANCE_POSITION:
			++maint;
			break;
	}
	//write
	FLASH_WriteData(RECORDS_LENGTH_ADDRESS, &record, INT_SIZE);
	FLASH_WriteData(LOCK_LENGTH_ADDRESS, &lock, INT_SIZE);
	FLASH_WriteData(MAINTENANCE_LENGTH_ADDRESS, &maint, INT_SIZE);
}

//sets up two arrays, one that will hold IDs of cards that open the lock and that can access maintenance
//other that will hold records of access
//WARNING: erases all records and cards saved
void HardResetFlash(){
	FLASH_EraseSectors(27, 29);
	SetUpControl();
	SetUpDefaultCard();
}

void AddRecord(struct record record){
	unsigned int *pointer = RECORDS_ADDRESS;
	unsigned int *length = RECORDS_LENGTH_ADDRESS;
	pointer += ((*length)*CHUNK_SIZE)/4;		// /4 because incrementing unsigned int pointer
	FLASH_WriteData(pointer, &record, sizeof(struct record));
	IncrementLength(RECORDS_POSITION);
}

void AddLockCard(unsigned int id){
	unsigned int *pointer = LOCK_ACCESS_ADDRESS;
	unsigned int *length = LOCK_LENGTH_ADDRESS;
	pointer += ((*length)*CHUNK_SIZE)/4;
	FLASH_WriteData(pointer, &id, INT_SIZE);
	IncrementLength(LOCK_POSITION);
}

void AddMaintenanceCard(unsigned int id){
	unsigned int *pointer = MAINTENANCE_ACCESS_ADDRESS;
	unsigned int *length = MAINTENANCE_LENGTH_ADDRESS;
	pointer += ((*length)*CHUNK_SIZE)/4;
	FLASH_WriteData(pointer, &id, INT_SIZE);
	IncrementLength(MAINTENANCE_POSITION);
}



