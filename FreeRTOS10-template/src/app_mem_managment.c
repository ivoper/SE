/*
 * app_mem_managment.c
 *
 *  Created on: 18/12/2018
 *      Author: A42172
 */

#include "app_mem_managment.h"
#include "e2prom.h"
#include <string.h>

//data structures addresses
#define CONTROL_ADDRESS				0x00	//16 bytes - 4 maintenance cards supported
#define	LOCK_ACCESS_ADDRESS			0x10	//240 bytes - 60 access cards supported
#define RECORDS_ADDRESS				0x100	//32 472 bytes - 738 records supported
#define ILLEGAL_RECORDS_ADDRESS		0x7FD8	//no more space left

#define MAINT_CARDS_MAX_LEN		4
#define MASTER_CARD_ID 0x1F016D86	//TODO garantir que ta certo

#define SIZEOF_RECORD	44
#define SIZEOF_ID		4

#define CARDS_LEN_POS	0
#define RECORDS_LEN_POS	2



//lengths in control set to 0, default maintenance card added
void SetupControl(){
	struct control ctrl = {.maintenanceCards[0] = MASTER_CARD_ID};
	E2P_Write(CONTROL_ADDRESS, sizeof(struct control), (unsigned char *)&ctrl);
}

void IncrementLength(unsigned short length, unsigned short pos){
	++length;
	E2P_Write(CONTROL_ADDRESS + pos, sizeof(unsigned short), (unsigned char *)&length);
}

unsigned short GetLength(unsigned short pos){
	unsigned short length;
	E2P_Read(CONTROL_ADDRESS+pos, sizeof(unsigned short), (unsigned char *)&length);
	return length;
}

void AddRecord(struct record record){
	unsigned short ix = GetLength(RECORDS_LEN_POS);
	E2P_Write(RECORDS_ADDRESS + (SIZEOF_RECORD*ix), SIZEOF_RECORD, (unsigned char *)&record);
	IncrementLength(ix, RECORDS_LEN_POS);
}

void AddLockCard(unsigned int id){
	unsigned short ix = GetLength(CARDS_LEN_POS);
	E2P_Write(LOCK_ACCESS_ADDRESS + (SIZEOF_ID*ix), SIZEOF_ID, (unsigned char *)&id);
	IncrementLength(ix, CARDS_LEN_POS);
}

int AdminCardExists(int id){
	struct control ctrl;
	E2P_Read(CONTROL_ADDRESS, sizeof(struct control), (unsigned char *)&ctrl);
	for(int i = 0; i<(MAINT_CARDS_MAX_LEN-1); ++i){
		if(ctrl.maintenanceCards[i] == id){
			return 1;
		}
	}
	return 0;
}

int CardExists(int id){
	unsigned short length = GetLength(CARDS_LEN_POS);
	unsigned int idSaved;
	for(int i = 0; i<length; ++i){
		E2P_Read(LOCK_ACCESS_ADDRESS + (SIZEOF_ID * i), SIZEOF_ID, (unsigned char *)&idSaved);
		if(idSaved == id){
			return 1;
		}
	}
	return 0;
}

unsigned int GetRecordsLength(){
	return GetLength(RECORDS_LEN_POS);
}

//app must assure ix exists with GetRecordsLength
struct record GetRecord(int ix){
	struct record rec;
	E2P_Read(RECORDS_ADDRESS + (SIZEOF_RECORD * ix), SIZEOF_RECORD, (unsigned char *)&rec);
	return rec;
}
