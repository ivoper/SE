/*
 * app.c
 *
 *  Created on: 30/05/2019
 *      Author: Ivo Pereira
 */

#include <stdio.h>
#include <time.h>
#include "LPC17xx.h"
#include "helper.h"
#include "app.h"
#include "states.h"
#include "keypados.h"
#include "lcdos.h"
#include "app_mem_managment.h"
#include "e2prom.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define PRIORITY_MAIN		2
#define PRIORITY_KEYPAD		4
#define PRIORITY_LCD		3
#define SMALL_STACK_SIZE	configMINIMAL_STACK_SIZE
#define APP_STACK_SIZE		512
#define QUEUE_LENGTH		6

//params for keypad task
struct kparams{
	SemaphoreHandle_t smphr;
	QueueHandle_t queue;
};

void MainFunc(void *params){
	struct tm date;
	InitializeApp(&date);
	unsigned int state = 0;
	Uid card;
	bool rewrite = true;
	unsigned int option;
	for(;;){
		switch(state){
			case IDLE_STATE:
				ShowCalendar(&date, &rewrite);
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
				rewrite = true;
				break;
			case CARD_INVALID_STATE:
				RegistInvalidAccess(&card);
				state = IDLE_STATE;
				rewrite = true;
				break;
			case MAINTENANCE_VERIFY_STATE:
				if(AssureHold()){
					WaitMaintenanceCard(&state, &card);
				}
				else{
					state = IDLE_STATE;
				}
				rewrite = true;
				break;
			case MAINTENANCE_CARD_READ_STATE:
				ValidateAdmin(&state, &card);
				rewrite = true;
				break;
			case MAINTENANCE_MENU_STATE:
				ShowMenu(&option);
				VerifyInput(&state, &option);
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
}

void LCDFunc(void *params){
	QueueHandle_t queue = (QueueHandle_t)params;
	LCDOS_Init(queue);
	for(;;){
		LCDOS_ProcessCmd();
	}
}

void KeypadFunc(void *params){
	int layout[12] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};
	struct kparams *p = (struct kparams *)params;
	SemaphoreHandle_t smphr = p->smphr;
	QueueHandle_t queue = p->queue;
	KEYPADOS_Init(layout, queue, smphr);
	for(;;){
		KEYPADOS_ProcessKey();
	}
}

void InitApp(){
	SemaphoreHandle_t keySemaphore = xSemaphoreCreateBinary();
	if(keySemaphore == NULL){
		printf("semaphore creation failed.\n");
	}
	QueueHandle_t keyQueue = xQueueCreate(QUEUE_LENGTH, sizeof(int));
	if(keyQueue == NULL){
		printf("queue creation failed.\n");
	}
	QueueHandle_t lcdQueue = xQueueCreate(QUEUE_LENGTH, sizeof(char)*CMD_SIZE);
	if(keyQueue == NULL){
		printf("queue creation failed.\n");
	}
	struct kparams *p = pvPortMalloc(sizeof(struct kparams));
	p->smphr = keySemaphore;
	p->queue = keyQueue;
	if(!xTaskCreate(KeypadFunc, "keypad", SMALL_STACK_SIZE, (void *)p, PRIORITY_KEYPAD, NULL)){
		printf("task creation failed.\n");
	}
	if(!xTaskCreate(LCDFunc, "lcd", SMALL_STACK_SIZE, (void *)lcdQueue, PRIORITY_LCD, NULL)){
		printf("task creation failed.\n");
	}

	/*
	if(!xTaskCreate(TestFunc, "test", APP_STACK_SIZE, NULL, PRIORITY_MAIN, NULL)){
		printf("task creation failed.\n");
	}
	**/

	if(!xTaskCreate(MainFunc, "main", APP_STACK_SIZE, NULL, PRIORITY_MAIN, NULL)){
		printf("task creation failed.\n");
	}
}
