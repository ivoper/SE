/*
 * helper.c
 *
 *  Created on: 17/12/2018
 *      Author: A42172
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "app_mem_managment.h"
#include "wait.h"
#include "keypad.h"
#include "rtc.h"
#include "lcd.h"
#include "helper.h"
#include "states.h"
#include "flash.h"
#include "led.h"

#define WAIT_ITERATIONS 5000

void InitializeApp(){
	int layout[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};
	WAIT_Init();
	LCDText_Init();
	KEYPAD_Init(layout);
	MFRC522_Init();
	RTC_Init(0);
	LED_Init(0, 22, 0);
}

void ResetFlash(){
	HardResetFlash();
}

//aux
//assume que uid não é maior que 4
unsigned int getUidUint(Uid* uidPtr){
	unsigned int val = 0;
	if (uidPtr->size > 4){
		printf("Não é garantido o funcionamento correto para cartões com uid maior que 4.");
	}
	for (int i=0; i<4; ++i){
		val |= (uidPtr->uidByte[i] << 8*i);
	}
	return val;
}

void RegistAccess(Uid *uid, struct tm *date, unsigned int valid){
	struct record record = {getUidUint(uid), *date, valid};
	AddRecord(record);
}

//IDLE
void ShowCalendar(){
	struct tm date;
	RTC_GetValue(&date);
	LCDText_Locate(0, 0);
	LCDText_Printf("%2.2d/%2.2d/%d %2.2d:%2.2d", date.tm_mday, date.tm_mon+1, 1900+date.tm_year, date.tm_hour, date.tm_min);
}

void WaitCard(unsigned int *state, Uid *uid){
	if(PICC_IsNewCardPresent()){
		//printf("card present\n");
		if(PICC_ReadCardSerial(uid)){
			//printf("card read\n");
			*state = CARD_READ_STATE;
		}
	}
}

void VerifyMaintenance(unsigned int *state){
	if(KEYPAD_Hit()){
		if(KEYPAD_Hit()){	//narrowing chance of hardware error
			if(KEYPAD_Read() == COMBINATION_CODE){
				*state = MAINTENANCE_VERIFY_STATE;
			}
		}
	}
}
// IDLE END

//CARD_READ
void ShowID(Uid *uid){
	LCDText_Clear();
	LCDText_Locate(0, 0);
	LCDText_Printf("%u", getUidUint(uid));
	LCDText_Locate(0,17);	//cursor out
}
void IsValid(unsigned int *state, Uid *uid){
	unsigned int length = *LOCK_LENGTH_ADDRESS;
	struct chunk *array = LOCK_ACCESS_ADDRESS;
	unsigned int id = getUidUint(uid);
	unsigned int *memid;
	int bool = 0;
	for (int i = 0; i<length; ++i){
		memid = (unsigned int *)&(array[i].byte[0]);
		if (id == *memid){
			bool = 1;
			break;
		}
	}
	LCDText_Locate(1, 0);
	if (bool){
		LCDText_Printf("Valid");
		LCDText_Locate(1, 17);
		*state = CARD_VALID_STATE;
	}
	else{
		LCDText_Printf("Invalid");
		LCDText_Locate(1, 17);
		*state = CARD_INVALID_STATE;
		WAIT_Ms(2000);		//in case of invalid lock wont open, show message and clear now
		LCDText_Clear();
	}
}
//CARD_READ end

//CARD_VALID
void OpenLock(){
	LED_On();
	WAIT_Ms(5000);
	LED_Off();
	LCDText_Clear();
}

void RegistValidAccess(Uid *uid){
	struct tm date;
	RTC_GetValue(&date);
	RegistAccess(uid, &date, 1);
}
//CARD_VALID end

//CARD_INVALID
void RegistInvalidAccess(Uid *uid){
	struct tm date;
	RTC_GetValue(&date);
	RegistAccess(uid, &date, 0);
}
//CARD_INVALID end

//MAINTENANCE_VERIFY
int AssureHold(){
	LCDText_Locate(1, 0);
	LCDText_Printf("Hold...");
	LCDText_Locate(1, 17);
	int abort = 0;
	for(int i = 0; i<4; ++i){
		WAIT_Ms(500);
		if(KEYPAD_Hit()){
			if(KEYPAD_Hit()){
				if(KEYPAD_Read() == COMBINATION_CODE){
					continue;
				}
			}
		}
		abort = 1;
		break;
	}
	if (abort == 1){
		LCDText_Clear();
		return 0;
	}
	return 1;
}

void WaitMaintenanceCard(unsigned int *state, Uid *uid){
	LCDText_Locate(1, 0);
	LCDText_Printf("Present card.");
	LCDText_Locate(1, 17);
	for (int i=0; i<WAIT_ITERATIONS; ++i){
		if(PICC_IsNewCardPresent()){
			if(PICC_ReadCardSerial(uid)){
				*state = MAINTENANCE_CARD_READ_STATE;
				return;
			}
		}
	}
	LCDText_Locate(1, 0);
	LCDText_Printf("No card found.");
	LCDText_Locate(1, 17);
	WAIT_Ms(1000);
	*state = IDLE_STATE;		//if maintenance isn't verified goes to idle
	LCDText_Clear();
}
//MAINTENANCE_VERIFY END

//MAINTENANCE_CARD_READ
void ValidateAdmin(unsigned int *state, Uid *uid){
	LCDText_Clear();
	LCDText_Locate(0, 0);
	LCDText_Printf("Card found.");
	LCDText_Locate(1, 0);
	unsigned int length = *MAINTENANCE_LENGTH_ADDRESS;
	struct chunk *array = MAINTENANCE_ACCESS_ADDRESS;
	unsigned int id = getUidUint(uid);
	unsigned int *memid;
	int bool = 0;
	for (int i = 0; i<length; ++i){
		memid = (unsigned int *)&(array[i].byte[0]);
		if (id == *memid){
			bool = 1;
			break;
		}
	}
	if (bool){
		LCDText_Printf("Valid");
		*state = MAINTENANCE_MENU_STATE;
	}
	else{
		LCDText_Printf("Invalid");
		*state = IDLE_STATE;
	}
	LCDText_Locate(1, 17);
	WAIT_Ms(1000);
	LCDText_Clear();
}
//MAINTENANCE_CARD_READ END

//MAINTENANCE_MENU
void ShowMenu(unsigned int *option){
	char *info = "info:8-up 0-down";
	char *op1 = "1-add card";
	char *op2 = "2-set clock";
	char *op3 = "3-set calendar";
	char *op4 = "4-get records";
	char *op5 = "*-exit";
	char *options[6] = {info, op1, op2, op3, op4, op5};
	int end = 0;		//bool
	unsigned int in;
	int i = 0;
	int l = 1;
	char *current1 = options[i];
	char *current2 = options[l];
	while(!end){
		LCDText_Clear();
		LCDText_Locate(0, 0);
		LCDText_Printf(current1);
		LCDText_Locate(1, 0);
		LCDText_Printf(current2);
		LCDText_Locate(1, 17);
		WAIT_Ms(50);	//avoid double movement in menu
		in = KEYPAD_Read();
		switch(in){
			case '8':
				if(i > 0){
					--i;
					--l;
				}
				break;
			case '0':
				if(l < 5){
					++i;
					++l;
				}
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '*':
				*option = in;
				end = 1;
				break;
		}
		current1 = options[i];
		current2 = options[l];
	}
}

void VerifyInput(unsigned int *state, unsigned int *option){
	switch(*option){
		case '1':
			*state = ADD_CARD_STATE;
			break;
		case '2':
			*state = CLOCK_CONFIG_STATE;
			break;
		case '3':
			*state = CALENDAR_CONFIG_STATE;
			break;
		case '4':
			*state = SHOW_RECORDS_STATE;
			break;
		case '*':
			*state = IDLE_STATE;
			break;
	}
	LCDText_Clear();
}
//MAINTENANCE_MENU END

//CLOCK_CONFIG
void ConfigClock(){
	struct tm date;
	RTC_GetValue(&date);
	LCDText_Locate(0, 0);
	LCDText_Printf("%2.2d:%2.2d:%2.2d", date.tm_hour, date.tm_min, date.tm_sec);
	int l;
	unsigned int val[2];	//two digits of val
	unsigned int in;
	unsigned int final[] = {date.tm_hour, date.tm_min, date.tm_sec};	//if user doesnt define a field, stays what it was
	for(int i=0; i<3; ++i){		//sec min hour -> 3
		l = 0;
		//if user doesnt define a field, stays what it was
		val[0] = final[i]/10;	//digit of tens
		val[1] = final[i]%10;	//digit of ones
		while(1){
			LCDText_Locate(0, i*3+l);
			WAIT_Ms(500);		//avoid fast input problems
			in = KEYPAD_Read();
			if(in == '#'){
				break;
			}
			LCDText_WriteChar(in);
			val[l] = in-'0';
			if (++l == 2){
				l=0;
			}
		}
		final[i] = val[1] + val[0]*10;
		if(i == 0){						//hour
			if(final[i]>23){				//invalid
				final[i] = date.tm_hour;
				--i;					//redo hour
				continue;
			}
			date.tm_hour = final[i];
		}
		else if(i == 1){				//min
			if(final[i]>59){				//invalid
				final[i] = date.tm_min;
				--i;					//redo min
				continue;
			}
			date.tm_min = final[i];
		}
		else{							//sec
			if(final[i]>59){				//invalid
				final[i] = date.tm_sec;
				--i;					//redo sec
				continue;
			}
			date.tm_sec = final[i];
		}
	}
	RTC_SetValue(&date);
}
//CLOCK_CONFIG END

//CALENDAR_CONFIG
void ConfigCalendar(){
	struct tm date;
	RTC_GetValue(&date);
	LCDText_Locate(0, 0);
	LCDText_Printf("%2.2d/%2.2d/%d", date.tm_mday, date.tm_mon+1, 1900+date.tm_year);
	int l;
	unsigned int val[4];	//two digits of val
	unsigned int in;
	unsigned int final[] = {date.tm_mday, date.tm_mon+1, date.tm_year};	//if user doesnt define a field, stays what it was
	for(int i=0; i<3; ++i){		//day mon year -> 3
		l = 0;
		//if user doesnt define a field, stays what it was
		if(i==2){	//year
			val[0] = final[i]/1000;	//digit of thousands
			val[1] = (final[i]/100)%10;	//digit of hundreds
			val[2] = (final[i]/10)%10;	//digit of tens
			val[3] = final[i]%10;	//digit of ones
		}else{
			val[0] = final[i]/10;	//digit of tens
			val[1] = final[i]%10;	//digit of ones
		}
		while(1){
			LCDText_Locate(0, i*3+l);
			WAIT_Ms(500);		//avoid fast input problems
			in = KEYPAD_Read();
			if(in == '#'){
				break;
			}
			LCDText_WriteChar(in);
			val[l] = in-'0';
			if ((++l == 2 && i != 2) || (l == 4 && i == 2)){	//i=2 is year -> 4 digits
				l=0;
			}
		}
		if(i == 0){						//day
			final[i] = val[1] + val[0]*10;
			if(final[i]>31){				//invalid
				final[i] = date.tm_mday;
				--i;					//redo day
				continue;
			}
			date.tm_mday = final[i];
		}
		else if(i == 1){				//mon
			final[i] = val[1] + val[0]*10 -1;
			if(final[i]>11){				//invalid
				final[i] = date.tm_mon+1;
				--i;					//redo mon
				continue;
			}
			date.tm_mon = final[i];
		}
		else{							//year
			final[i] = val[3] + val[2]*10 + val[1]*100 + val[0]*1000;
			final[i] -= 1900;
			date.tm_year = final[i];
		}
	}
	RTC_SetValue(&date);
}
//CALENDAR_CONFIG END

//SHOW_RECORDS
void ShowRecords(){
	unsigned int length = *RECORDS_LENGTH_ADDRESS;
	struct chunk *array = RECORDS_ADDRESS;
	struct record *record;
	struct tm date;
	unsigned int val;
	for(int i = 0; ; ++i){
		if(!(i<length)){
			i=0;
		}
		record = (struct record *)&(array[i].byte[0]);
		LCDText_Clear();
		LCDText_Locate(0, 0);
		LCDText_Printf("%d-%s", record->cardId, record->valid ? "valid" : "inval.");
		date = record->date;
		LCDText_Locate(1, 0);
		LCDText_Printf("%2.2d/%2.2d/%d %2.2d:%2.2d", date.tm_mday, date.tm_mon+1, 1900+date.tm_year, date.tm_hour, date.tm_min);
		WAIT_Ms(500);		//avoid fast input problems
		val = KEYPAD_Read();
		while(val != '*' && val != '#'){
			val = KEYPAD_Read();
		}
		if (val == '#'){
			break;
		}
	}
	LCDText_Clear();
}
//SHOW_RECORDS END

//ADD_CARD
void WaitCardToAdd(unsigned int *state, Uid *uid){
	LCDText_Locate(0, 0);
	LCDText_Printf("Present card.");
	LCDText_Locate(0, 17);
	for (int i=0; i<WAIT_ITERATIONS; ++i){
		if(PICC_IsNewCardPresent()){
			if(PICC_ReadCardSerial(uid)){
				*state = ADD_CARD_READ_STATE;
				return;
			}
		}
	}
	LCDText_Locate(0, 0);
	LCDText_Printf("No card found.");
	LCDText_Locate(0, 17);
	*state = MAINTENANCE_MENU_STATE;
	WAIT_Ms(1000);
}
//ADD_CARD END

//ADD_CARD_READ
void VerifyExistance(unsigned int *state, Uid *uid){
	LCDText_Clear();
	unsigned int length = *LOCK_LENGTH_ADDRESS;
	struct chunk *array = LOCK_ACCESS_ADDRESS;
	unsigned int id = getUidUint(uid);
	unsigned int *memid;
	int repeat = 0;
	for (int i = 0; i<length; ++i){
		memid = (unsigned int *)&(array[i].byte[0]);
		if (id == *memid){
			repeat = 1;
			break;
		}
	}
	if (repeat){
		LCDText_Locate(0, 0);
		LCDText_Printf("Card already");
		LCDText_Locate(1, 0);
		LCDText_Printf("registered");
		*state = MAINTENANCE_MENU_STATE;
	}
	else{
		LCDText_Locate(0, 0);
		LCDText_Printf("Card found.");
		*state = ADD_CARD_CONFIRMATION_STATE;
	}
	LCDText_Locate(1, 17);
	WAIT_Ms(1000);
}
//ADD_CARD_READ END

//ADD_CARD_CONFIRMATION
void ConfirmRegist(Uid *uid){
	LCDText_Clear();
	LCDText_Locate(0, 0);
	unsigned int id = getUidUint(uid);
	LCDText_Printf("%d", id);
	LCDText_Locate(1, 0);
	LCDText_Printf("confirm: y-* n-#");
	unsigned int val = KEYPAD_Read();
	while(val != '*' && val != '#'){
		val = KEYPAD_Read();
	}
	if (val == '*'){
		AddLockCard(id);
	}
}
//ADD_CARD_CONFIRMATION END




