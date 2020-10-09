/*
===============================================================================
 Name        : Lab_2.c
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
#include "keypad.h"
#include "led.h"
#include "wait.h"

void testHit(){
	while(1){
		printf("%d", KEYPAD_Hit());
	}
}

void testRead(){
	while(1){
		printf("%d", KEYPAD_Read());
	}
}

void blink(int n){
	while(n>0){
		LED_On();
		WAIT_Ms(200);
		LED_Off();
		WAIT_Ms(200);
		--n;
	}
}

int main(void) {
	LED_Init(0, 22, 0);
	int array[12] = {5, 6, 7, 8, 9, 10, 11, 12, 14, 15, 16, 17};
	KEYPAD_Init(array);
	WAIT_Init();
	while(1){
		int val = KEYPAD_Read();
		if (val == KEY_COMBINATION_UNSUPPORTED_CODE)
			continue;
		blink(val);
	}
	//testHit();
	//testRead();
    return 0;
}
