/*
===============================================================================
 Name        : Lab_3.c
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
#include "lcd.h"
#include "wait.h"

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

int main(void) {
	WAIT_Init();
    LCDText_Init();
    LCDText_WriteString("Whew.");
    LCDText_Locate(0, 0);
    LCDText_Printf("%d = a 1", 1);
    while(1) {
    	WAIT_Ms(2000);
    	LCDText_Clear();
    	LCDText_WriteChar('A');
    }
    return 0 ;
}
