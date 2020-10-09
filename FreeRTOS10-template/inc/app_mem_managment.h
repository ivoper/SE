/*
 * app_mem_managment.h
 *
 *  Created on: 18/12/2018
 *      Author: A42172
 */

#ifndef APP_MEM_MANAGMENT_H_
#define APP_MEM_MANAGMENT_H_

#include <time.h>

struct record{
	unsigned int cardId;
	struct tm date;
	char valid;
};

struct control{
	unsigned short cardsIx;
	unsigned short recordsIx;
	unsigned int maintenanceCards[3];
};

void SetupControl();

void AddRecord(struct record record);
void AddLockCard(unsigned int id);

int CardExists(int id);
int AdminCardExists(int id);

unsigned int GetRecordsLength();
struct record GetRecord(int ix);

#endif /* APP_MEM_MANAGMENT_H_ */
