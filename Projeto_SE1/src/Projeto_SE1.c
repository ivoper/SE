/*
===============================================================================
 Name        : Projeto_SE1.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>

#include "helper.h"
#include "states.h"
#include "mfrc522.h"

unsigned int state = 0;
Uid card;

int main(void) {
	ResetFlash();
	InitializeApp();
	int end = 0;
	unsigned int option;
	while(!end){
		switch(state){
			case IDLE_STATE:
				ShowCalendar();
				WaitCard(&state, &card);
				VerifyMaintenance(&state);
				break;
			case CARD_READ_STATE:
				ShowID(&card);
				IsValid(&state, &card);
				break;
			case CARD_VALID_STATE:
				OpenLock();
				RegistValidAccess(&card);
				state = IDLE_STATE;
				break;
			case CARD_INVALID_STATE:
				RegistInvalidAccess(&card);
				state = IDLE_STATE;
				break;
			case MAINTENANCE_VERIFY_STATE:
				if(AssureHold()){
					WaitMaintenanceCard(&state, &card);
				}
				else{
					state = IDLE_STATE;
				}
				break;
			case MAINTENANCE_CARD_READ_STATE:
				ValidateAdmin(&state, &card);
				break;
			case MAINTENANCE_MENU_STATE:
				ShowMenu(&option);
				VerifyInput(&state, &option);
				break;
			case CLOCK_CONFIG_STATE:
				ConfigClock();
				state = MAINTENANCE_MENU_STATE;
				break;
			case CALENDAR_CONFIG_STATE:
				ConfigCalendar();
				state = MAINTENANCE_MENU_STATE;
				break;
			case SHOW_RECORDS_STATE:
				ShowRecords();
				state = MAINTENANCE_MENU_STATE;
				break;
			case ADD_CARD_STATE:
				WaitCardToAdd(&state, &card);
				break;
			case ADD_CARD_READ_STATE:
				VerifyExistance(&state, &card);
				break;
			case ADD_CARD_CONFIRMATION_STATE:
				ConfirmRegist(&card);
				state = MAINTENANCE_MENU_STATE;
				break;
			default:
				state = 0;
				break;
		}
	}
    return 0 ;
}
