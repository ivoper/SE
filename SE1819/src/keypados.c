/*
 * keypados.c
 *
 *  Created on: 04/06/2019
 *      Author: Ivo Pereira
 */

#include "keypados.h"
#include "LPC17xx.h"
#include "waitos.h"
#include <string.h>

#define BITS_TO_PINMODE (0x3f << 14)	//bits in correct position to zero in pinmode

#define INPUT_MASK	(0x7 << 23)	// input pins for keyboard
#define OUTPUT_MASK	(0xF << 6)	// output pins for keyboard
#define SHIFT_TO_OUTPUT 6
#define SHIFT_TO_INPUT 23

#define LAYOUT_SIZE 		12
#define VAL_TO_XOR			0x07	//seek explanation

int Layout[LAYOUT_SIZE];
SemaphoreHandle_t Semaphore;
QueueHandle_t Queue;

void EINT3_IRQHandler(){
	NVIC_DisableIRQ(EINT3_IRQn);
	if (!LPC_GPIOINT->IntStatus){		//when initializing there is an interrupt triggered
		NVIC_EnableIRQ(EINT3_IRQn);
		return;
	}
	LPC_GPIOINT->IO0IntClr |= INPUT_MASK;	//clear interrupt
	BaseType_t TaskWoken = pdFALSE;
	if(xSemaphoreGiveFromISR(Semaphore, &TaskWoken) == pdPASS){				//resumes keypad task
		if(TaskWoken == pdTRUE){
			portYIELD_FROM_ISR(TaskWoken);
			return;
		}
	}
	xSemaphoreTakeFromISR(Semaphore, &TaskWoken);	//if give fails or no thread was waiting, block semaphore again
	portYIELD_FROM_ISR(TaskWoken);
	NVIC_EnableIRQ(EINT3_IRQn);
}

//call from keypad task
void KEYPADOS_Init(int layout[12], QueueHandle_t queue, SemaphoreHandle_t smphr){
	Queue = queue;
	Semaphore = smphr;
	LPC_PINCON->PINMODE1 &= ~(BITS_TO_PINMODE);		//assures input bits are in pull up mode
	LPC_GPIO0->FIODIR &= ~(INPUT_MASK);		//bits for input to 0
	LPC_GPIO0->FIODIR |= OUTPUT_MASK;		//bits for output to 1
	memcpy(Layout, layout, LAYOUT_SIZE * sizeof(int));
	LPC_GPIO0->FIOCLR = OUTPUT_MASK;		//send all 0s
	LPC_GPIOINT->IO0IntEnF |= INPUT_MASK;	//enable int in input bits
	NVIC_SetPriority(EINT3_IRQn, 5);		//priority to 5, safe to use in freeRTOS interupts
	LPC_GPIOINT->IO0IntClr |= INPUT_MASK;	//just to be sure
	NVIC_EnableIRQ(EINT3_IRQn);
}

void KEYPADOS_ProcessKey(){
	xSemaphoreTake(Semaphore, portMAX_DELAY);
	int key = KEYPADOS_PoolKeypad();
	if(key != -1){
		xQueueSendToBack(Queue, &key, 0);
	}
	NVIC_EnableIRQ(EINT3_IRQn);		//only re-enable ints here, even though ints are falling edge because of multiple keys pressed
}

int KEYPADOS_PoolKeypad(){
	WAITOS_Ms(30);		//for combination of keys
	int ret = KEY_COMBINATION_UNSUPPORTED_CODE;
	for(int i=0; i<NUMBER_OF_LINES; ++i){
		LPC_GPIO0->FIOSETL = OUTPUT_MASK;	//output bits to 1
		LPC_GPIO0->FIOCLRL = 1<<(SHIFT_TO_OUTPUT + i);	//line output bit to 0
		int col = (LPC_GPIO0->FIOPIN & INPUT_MASK)>>SHIFT_TO_INPUT;	//input bits(column bits) to col
		if(col != 7){		//if not all 1s, is the line
			if ((i == NUMBER_OF_LINES - 1) && (col == 2)){		//combination supported in last line(row) of keypad -> 010
				ret = COMBINATION_CODE;
			}
			if (col == NUMBER_OF_COLUMNS){			//if key is pressed in last column then the col value is correct
				ret = Layout[col + (i*(NUMBER_OF_LINES -1)) - 1];		//col 3
			}
			col ^= VAL_TO_XOR;					//if key is pressed in other columns this xor puts correct value in col
			if (col < NUMBER_OF_COLUMNS){		//can only be col 1 or 2
				ret = Layout[col + (i*(NUMBER_OF_LINES -1)) - 1];
			}
			break;		//is the line, but no supported key
		}
	}
	LPC_GPIO0->FIOCLRL = OUTPUT_MASK;
	return ret;
}

//called from fetcher task
int KEYPADOS_GetKey(TickType_t ticks){
	int key;
	if(xQueueReceive(Queue, &key, ticks) == pdPASS){
		return key;
	}
	return -1;
}
