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
#include "waitos.h"
#include "rtc.h"
#include "lcdos.h"
#include "helper.h"
#include "states.h"
#include "flash.h"
#include "led.h"
#include "keypados.h"
#include "e2prom.h"
#include "network.h"
#include "esp.h"

#define WAIT_ITERATIONS 2000
#define SSID	"HUAWEI P8 lite 2017"
#define PASS	"pass1234"

void InitializeApp(struct tm* date){
	MFRC522_Init();
	LED_Init(0, 22, 0);
	E2P_Init();
	ESP_Init();
	time_t time;
	if(ConnectToAP(SSID, PASS)){
		time = GetCurrentTime();
		RTC_Init(time);
		RTC_GetValue(date);
	}
	//SetupControl();
}

//aux
//assume que uid não é maior que 4
unsigned int getUidUint(Uid* uidPtr){
	unsigned int val = 0;
	if (uidPtr->size > 4){
		val = -1;
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
void ShowCalendar(struct tm *date, bool *rewrite){
	struct tm newDate;
	RTC_GetValue(&newDate);
	if(newDate.tm_min != date->tm_min || *rewrite){
		*rewrite = false;
		memcpy(date, &newDate, sizeof(struct tm));
		LCDOS_Locate(0, 0);
		LCDOS_Printf("%2.2d/%2.2d/%d %2.2d:%2.2d", date->tm_mday, date->tm_mon+1, 1900+date->tm_year, date->tm_hour, date->tm_min);
	}
}

void WaitCard(unsigned int *state, Uid *uid){
	if(PICC_IsNewCardPresent()){
		if(PICC_ReadCardSerial(uid)){
			*state = CARD_READ_STATE;
		}
	}
}

void VerifyMaintenance(unsigned int *state){
	int key = KEYPADOS_GetKey(0);
	if(key == COMBINATION_CODE){
		*state = MAINTENANCE_VERIFY_STATE;
	}
}
// IDLE END

//CARD_READ
void ShowID(Uid *uid){
	LCDOS_Clear();
	LCDOS_Locate(0, 0);
	LCDOS_Printf("%u", getUidUint(uid));
	LCDOS_Locate(0,17);	//cursor out
}
void IsValid(unsigned int *state, Uid *uid){
	unsigned int id = getUidUint(uid);
	int valid = CardExists(id);
	LCDOS_Locate(1, 0);
	if (valid){
		LCDOS_Printf("Valid");
		LCDOS_Locate(1, 17);
		*state = CARD_VALID_STATE;
	}
	else{
		LCDOS_Printf("Invalid");
		LCDOS_Locate(1, 17);
		*state = CARD_INVALID_STATE;
		WAITOS_Ms(2000);		//in case of invalid lock wont open, show message and clear now
		LCDOS_Clear();
	}
}
//CARD_READ end

//CARD_VALID
void OpenLock(){
	LED_On();
	WAITOS_Ms(5000);
	LED_Off();
	LCDOS_Clear();
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
	LCDOS_Locate(1, 0);
	LCDOS_Printf("Hold...");
	LCDOS_Locate(1, 17);
	int abort = 0;
	for(int i = 0; i<4; ++i){
		WAITOS_Ms(500);
		if(KEYPADOS_PoolKeypad() == COMBINATION_CODE){		//see if holding instead of getting another key
			continue;
		}
		abort = 1;
		break;
	}
	if (abort == 1){
		LCDOS_Clear();
		return 0;
	}
	return 1;
}

void WaitMaintenanceCard(unsigned int *state, Uid *uid){
	LCDOS_Locate(1, 0);
	LCDOS_Printf("Present card.");
	LCDOS_Locate(1, 17);
	for (int i=0; i<WAIT_ITERATIONS; ++i){
		if(PICC_IsNewCardPresent()){
			if(PICC_ReadCardSerial(uid)){
				*state = MAINTENANCE_CARD_READ_STATE;
				return;
			}
		}
	}
	LCDOS_Locate(1, 0);
	LCDOS_Printf("No card found.");
	LCDOS_Locate(1, 17);
	WAITOS_Ms(1000);
	*state = IDLE_STATE;		//if maintenance isn't verified goes to idle
	LCDOS_Clear();
}
//MAINTENANCE_VERIFY END

//MAINTENANCE_CARD_READ
void ValidateAdmin(unsigned int *state, Uid *uid){
	LCDOS_Clear();
	LCDOS_Locate(0, 0);
	LCDOS_Printf("Card found.");
	LCDOS_Locate(1, 0);
	unsigned int id = getUidUint(uid);
	int valid = AdminCardExists(id);
	if (valid){
		LCDOS_Printf("Valid");
		*state = MAINTENANCE_MENU_STATE;
	}
	else{
		LCDOS_Printf("Invalid");
		*state = IDLE_STATE;
	}
	LCDOS_Locate(1, 17);
	WAITOS_Ms(1000);
	LCDOS_Clear();
}
//MAINTENANCE_CARD_READ END

//MAINTENANCE_MENU
void ShowMenu(unsigned int *option){
	char *info = "info:8-up 0-down";
	char *op1 = "1-add card";
	char *op2 = "2-get records";
	char *op3 = "*-exit";
	char *options[4] = {info, op1, op2, op3};
	int end = 0;		//bool
	int in;
	int i = 0;
	int l = 1;
	char *current1 = options[i];
	char *current2 = options[l];
	while(!end){
		LCDOS_Clear();
		LCDOS_Locate(0, 0);
		LCDOS_Printf(current1);
		LCDOS_Locate(1, 0);
		LCDOS_Printf(current2);
		LCDOS_Locate(1, 17);
		//WAITOS_Ms(50);	//avoid double movement in menu
		in = KEYPADOS_GetKey(portMAX_DELAY);
		switch(in){
			case '8':
				if(i > 0){
					--i;
					--l;
				}
				break;
			case '0':
				if(l < 3){
					++i;
					++l;
				}
				break;
			case '1':
			case '2':
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
			*state = SHOW_RECORDS_STATE;
			break;
		case '*':
			*state = IDLE_STATE;
			break;
	}
	LCDOS_Clear();
}
//MAINTENANCE_MENU END

//SHOW_RECORDS
void ShowRecords(){
	unsigned int length = GetRecordsLength();
	if(length < 1){
		LCDOS_Clear();
		LCDOS_Locate(0, 0);
		LCDOS_Printf("No records");
		WAITOS_Ms(2000);
		LCDOS_Clear();
		return;
	}
	struct record record;
	struct tm date;
	unsigned int val;
	for(int i = 0; ; ++i){
		if(!(i<length)){
			i=0;
		}
		record = GetRecord(i);
		LCDOS_Clear();
		LCDOS_Locate(0, 0);
		LCDOS_Printf("%d-%s", record.cardId, record.valid ? "valid" : "inval.");
		date = record.date;
		LCDOS_Locate(1, 0);
		LCDOS_Printf("%2.2d/%2.2d/%d %2.2d:%2.2d", date.tm_mday, date.tm_mon+1, 1900+date.tm_year, date.tm_hour, date.tm_min);
		//WAITOS_Ms(500);		//avoid fast input problems
		do{
			val = KEYPADOS_GetKey(portMAX_DELAY);
		}
		while(val != '*' && val != '#');
		if (val == '#'){
			break;
		}
	}
	LCDOS_Clear();
}
//SHOW_RECORDS END

//ADD_CARD
void WaitCardToAdd(unsigned int *state, Uid *uid){
	LCDOS_Locate(0, 0);
	LCDOS_Printf("Present card.");
	LCDOS_Locate(0, 17);
	for (int i=0; i<WAIT_ITERATIONS; ++i){
		if(PICC_IsNewCardPresent()){
			if(PICC_ReadCardSerial(uid)){
				*state = ADD_CARD_READ_STATE;
				return;
			}
		}
	}
	LCDOS_Locate(0, 0);
	LCDOS_Printf("No card found.");
	LCDOS_Locate(0, 17);
	*state = MAINTENANCE_MENU_STATE;
	WAITOS_Ms(1000);
}
//ADD_CARD END

//ADD_CARD_READ
void VerifyExistance(unsigned int *state, Uid *uid){
	LCDOS_Clear();
	unsigned int id = getUidUint(uid);
	int repeat = CardExists(id);
	if (repeat){
		LCDOS_Locate(0, 0);
		LCDOS_Printf("Card already");
		LCDOS_Locate(1, 0);
		LCDOS_Printf("registered");
		*state = MAINTENANCE_MENU_STATE;
	}
	else{
		LCDOS_Locate(0, 0);
		LCDOS_Printf("Card found.");
		*state = ADD_CARD_CONFIRMATION_STATE;
	}
	LCDOS_Locate(1, 17);
	WAITOS_Ms(1000);
}
//ADD_CARD_READ END

//ADD_CARD_CONFIRMATION
void ConfirmRegist(Uid *uid){
	LCDOS_Clear();
	LCDOS_Locate(0, 0);
	unsigned int id = getUidUint(uid);
	LCDOS_Printf("%d", id);
	LCDOS_Locate(1, 0);
	LCDOS_Printf("confirm: y-* n-#");
	int val;
	do{
		val = KEYPADOS_GetKey(portMAX_DELAY);
	}
	while(val != '*' && val != '#');
	if (val == '*'){
		AddLockCard(id);
	}
}
//ADD_CARD_CONFIRMATION END




