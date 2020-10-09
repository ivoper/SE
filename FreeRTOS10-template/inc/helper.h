/*
 * helper.h
 *
 *  Created on: 17/12/2018
 *      Author: A42172
 */

#ifndef HELPER_H_
#define HELPER_H_

#include "mfrc522.h"
#include <stdbool.h>

void InitializeApp(struct tm* date);
void ResetFlash();

//idle
void ShowCalendar(struct tm *date, bool *rewrite);
void WaitCard(unsigned int *state, Uid * uid);
void VerifyMaintenance(unsigned int *state);

//card_read
void ShowID(Uid *uid);
void IsValid(unsigned int *state, Uid *uid);

//card_valid
void OpenLock();
void RegistValidAccess(Uid *uid);

//card_invalid
void RegistInvalidAccess(Uid *uid);

//maintenance_verify
int AssureHold();
void WaitMaintenanceCard(unsigned int *state, Uid *uid);

//maintenance_card_read
void ValidateAdmin(unsigned int *state, Uid *uid);

//maintenance_menu
void ShowMenu(unsigned int *option);
void VerifyInput(unsigned int *state, unsigned int *option);

//clock_config
void ConfigClock();

//calendar_config
void ConfigCalendar();

//show_records
void ShowRecords();

//add_card
void WaitCardToAdd(unsigned int *state, Uid *uid);

//add_card_read
void VerifyExistance(unsigned int *state, Uid *uid);

//add_card_confirmation
void ConfirmRegist(Uid *uid);

#endif /* HELPER_H_ */
