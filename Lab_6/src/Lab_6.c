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
#include "spi.h"
#include "mfrc522.h"
#include "mfrc.h"
#include "lcd.h"
#include "wait.h"
#include "flash.h"

#define INPUT_MASK 			1<<26
#define SECTOR_29_ADDRESS	0x00078000
#define UID_SIZE			4

//assume que uid não é maior que 4
unsigned int getUidUint(Uid* uidPtr){
	unsigned int val = 0;
	if (uidPtr->size > 4){
		printf("Não é garantido o funcionamento correto para cartões com uid maior que 4.");
	}
	for (int i=0; i<UID_SIZE; ++i){
		val |= (uidPtr->uidByte[i] << 8*i);
	}
	return val;
}

void saveInFlash(Uid* uidPtr){
	unsigned int val = getUidUint(uidPtr);
	FLASH_EraseSectors(29, 29);
	FLASH_WriteData((unsigned int *)SECTOR_29_ADDRESS, &val, 4);
}

int main(void) {
	LCDText_Init();
	MFRC522_Init();
	WAIT_Init();
	Uid uid;
	unsigned int *addr = (unsigned int *)SECTOR_29_ADDRESS;
	while(1){
		LCDText_Locate(0, 0);
		LCDText_Printf("Memory:%u", *addr);
		while(!PICC_IsNewCardPresent()){
			printf("try\n");
		}
		printf("card present\n");
		int result = PICC_ReadCardSerial(&uid);
		if(result){
			printf("card read\n");
			LCDText_Locate(1, 0);
			LCDText_Printf("UID:%u", getUidUint(&uid));
			saveInFlash(&uid);
			WAIT_Ms(1000);
		}
	}
    return 0 ;
}
