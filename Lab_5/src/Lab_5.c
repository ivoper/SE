/*
===============================================================================
 Name        : Lab_5.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LPC17xx.h"
#include "spi.h"
#include "mfrc522.h"
#include "mfrc.h"
#include "lcd.h"
#include "wait.h"

#define INPUT_MASK 1<<26

void print_inLCD(Uid* uidPtr){
	char string[50] = "";
	char aux[10] = "";
	for(int i = 0; i<uidPtr->size; ++i){
		itoa(uidPtr->uidByte[i], aux, 10);
		strcat(string, aux);
	}

	LCDText_Printf("%s", string);
}

int main(void) {
	LCDText_Init();
	MFRC522_Init();
	WAIT_Init();
	Uid *uidPtr = (Uid*)malloc(sizeof(Uid));
	while(1){
		while(!PICC_IsNewCardPresent()){
			printf("try\n");
		}
		printf("card present\n");
		int result = PICC_ReadCardSerial(uidPtr);
		if(result){
			printf("card read\n");
			print_inLCD(uidPtr);
			WAIT_Ms(1000);
		}
	}
    return 0 ;
}
