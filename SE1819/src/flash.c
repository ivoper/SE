/*
 * flash.c
 *
 *  Created on: 10/12/2018
 *      Author: A42172
 */

#include "flash.h"
#include "LPC17xx.h"
#include "string.h"

#define IAP_LOCATION	0x1FFF1FF1

#define SMALL_SECTOR_SIZE		0x1000		//4kb
#define BIG_SECTOR_SIZE			0x8000		//32kb
#define START_BIG_ADDRESS		0x00010000
#define END_BIG_ADDRESS			0x0007FFFF
#define NUMBER_SMALL_SECTORS	16
#define CHUNK_SIZE				256

#define PREPARE_SECTOR_COMMAND	50
#define COPY_COMMAND			51
#define ERASE_SECTOR_COMMAND	52
#define COMPARE_COMMAND			56

#define CMD_SUCCESS		0
#define BUSY			11
#define INVALID_SECTOR	7


typedef void (*IAP)(unsigned int [],unsigned int[]);

//executes an IAP command
unsigned int ExecuteIAP(unsigned int* commandTable, unsigned int* outputTable){
	IAP iap_entry;
	iap_entry = (IAP)IAP_LOCATION;
	iap_entry(commandTable, outputTable);	//calls IAP command
	return outputTable[0];					//returns status code
}

//gets the start and end sector given an address and size
void GetSectors(unsigned int dstAddr, unsigned int size, unsigned int *startSector, unsigned int *endSector){
	if(dstAddr < START_BIG_ADDRESS){		//is in 4k sector
		*startSector = (dstAddr & 0xF000)/SMALL_SECTOR_SIZE;	//alligns
		unsigned int endAddr = dstAddr+size;
		if (endAddr > START_BIG_ADDRESS){	//end sector is 4k or 32k
			*endSector =  NUMBER_SMALL_SECTORS + ((endAddr & 0xFF000)/BIG_SECTOR_SIZE) -2;	//get number of 32k sector
		}
		*endSector = (endAddr & 0xF000)/SMALL_SECTOR_SIZE;									//get number of 4k sector
	}
	else{								//is in 32k sector
		*startSector = NUMBER_SMALL_SECTORS + ((dstAddr & 0xFF000)/BIG_SECTOR_SIZE) - 2;
		unsigned int endAddr = dstAddr+size;
		*endSector =  NUMBER_SMALL_SECTORS + ((endAddr & 0xFF000)/BIG_SECTOR_SIZE) -2;
	}
}

//executes the prepare sector command, used before erase and write operations
unsigned int PrepareSector(unsigned int startSector, unsigned int endSector){
	unsigned int in[3] = {PREPARE_SECTOR_COMMAND, startSector, endSector};
	unsigned int out[1] = {};
	return ExecuteIAP(in, out);		//returns status code
}

unsigned int FLASH_EraseSectors(unsigned int startSector, unsigned int endSector){
	unsigned int errorCode = PrepareSector(startSector, endSector);
	if(errorCode != CMD_SUCCESS){
		return errorCode;
	}
	unsigned int in[4] = {ERASE_SECTOR_COMMAND, startSector, endSector, SystemCoreClock/1000};
	unsigned int out[1] = {};
	ExecuteIAP(in, out);
	return out[0];
}

static unsigned char buffer[CHUNK_SIZE];

unsigned int FLASH_WriteData(void *dstAddr, void *srcAddr, unsigned int size){
	unsigned int startSector, endSector;
	unsigned int toCopy = 0;
	while(size>toCopy){
		toCopy+=CHUNK_SIZE;
	}

	GetSectors((unsigned int)dstAddr, toCopy, &startSector, &endSector);	//discover sectors
	unsigned int errorCode = PrepareSector(startSector, endSector);
	if(errorCode != CMD_SUCCESS){
		return errorCode;
	}

	unsigned int in[5] = {COPY_COMMAND, (unsigned int)dstAddr, (unsigned int)buffer, CHUNK_SIZE, SystemCoreClock/1000};
	unsigned int out[1] = {};
	while(toCopy > 0){
		memcpy(buffer, dstAddr, CHUNK_SIZE);
		memcpy(buffer, srcAddr, (size > CHUNK_SIZE) ? CHUNK_SIZE : size);
		in[1] = (unsigned int)dstAddr;
		ExecuteIAP(in, out);
		if(out[0] != 0){
			return out[0];
		}
		else{
			srcAddr += CHUNK_SIZE;
			dstAddr += CHUNK_SIZE;
			toCopy -= CHUNK_SIZE;
		}
	}
	return out[0];
}

unsigned int FLASH_VerifyData(void *dstAddr, void *srcAddr, unsigned int size){
	unsigned int in[4] = {COMPARE_COMMAND, (unsigned int)dstAddr, (unsigned int)srcAddr, size};
	unsigned int out[2] = {};
	ExecuteIAP(in, out);
	return out[0];
}
