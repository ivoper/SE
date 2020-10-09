/*
===============================================================================
 Name        : Lab_4.c
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
#include <time.h>
#include "rtc.h"
#include "lcd.h"

int main(void) {
	time_t seconds;
	time(&seconds);
	RTC_Init(seconds);
	LCDText_Init();
	struct tm time;
	RTC_GetValue(&time);
	while(1){
		LCDText_Locate(0, 0);
		RTC_GetValue(&time);
		LCDText_Printf("%i:%i:%i", time.tm_hour, time.tm_min, time.tm_sec);
	}
}
