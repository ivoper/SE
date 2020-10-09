/*
===============================================================================
 Name        : Lab_1.c
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
#include "wait.h"
#include "led.h"

int main(void) {
	WAIT_Init();
	LED_Init(0, 22, 0);
	while(1){
		LED_On();
		WAIT_Ms(500);
		LED_Off();
		WAIT_Ms(500);
	}
}
