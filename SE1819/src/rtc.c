/*
 * rtc.c
 *
 *  Created on: 30/10/2018
 *      Author: A42172
 */

#include "rtc.h"
#include "LPC17xx.h"

#define BIT_IN_PCONP 9
#define CTIME_SECONDS_MASK	0x3F
#define CTIME_MINUTES_MASK	0x3F00
#define CTIME_HOUR_MASK		0x1F0000
#define CTIME_DOM_MASK		0x1F
#define CTIME_MONTH_MASK	0xF00
#define CTIME_YEAR_MASK		0xFFF0000
#define CTIME_DOW_MASK		0x7000000
#define CTIME_DOY_MASK		0xFFF

#define MINUTES_SHIFT	2*4
#define HOUR_SHIFT		4*4
#define DOW_SHIFT		6*4
#define MONTH_SHIFT		2*4
#define YEAR_SHIFT		4*4

void RTC_Init(time_t seconds){
	LPC_SC->PCONP |= 1<<BIT_IN_PCONP;
	LPC_RTC->CCR = 0x2;			//stops rtc
	LPC_RTC->ILR = 0x3;			//clear interupts
	LPC_RTC->ILR = 0x0;			//disable interupts
	LPC_RTC->CIIR = 0x00;		//assures increment interrupts disabled
	LPC_RTC->AMR = 0xFF;		//disable alarm
	RTC_SetSeconds(seconds);
	LPC_RTC->CCR = 0x1;			//starts rtc
}

void RTC_GetValue(struct tm *dateTime){
	unsigned int ctime = LPC_RTC->CTIME0;		//first read
	dateTime->tm_sec = ctime & CTIME_SECONDS_MASK;
	dateTime->tm_min = (ctime & CTIME_MINUTES_MASK) >> MINUTES_SHIFT;
	dateTime->tm_hour = (ctime & CTIME_HOUR_MASK) >> HOUR_SHIFT;
	dateTime->tm_wday = (ctime & CTIME_DOW_MASK) >> DOW_SHIFT;

	ctime = LPC_RTC->CTIME1;					//second read
	dateTime->tm_mday = ctime & CTIME_DOM_MASK;
	dateTime->tm_mon = (ctime & CTIME_MONTH_MASK) >> MONTH_SHIFT;
	dateTime->tm_year = (ctime & CTIME_YEAR_MASK) >> YEAR_SHIFT;

	ctime = LPC_RTC->CTIME2;					//third read
	dateTime->tm_yday = ctime & CTIME_DOY_MASK;
}

void RTC_SetValue(struct tm *dateTime){
	LPC_RTC->SEC = dateTime->tm_sec;
	LPC_RTC->MIN = dateTime->tm_min;
	LPC_RTC->HOUR = dateTime->tm_hour;
	LPC_RTC->DOM = dateTime->tm_mday;
	LPC_RTC->MONTH = dateTime->tm_mon;
	LPC_RTC->YEAR = dateTime->tm_year;
	LPC_RTC->DOW = dateTime->tm_wday;
	LPC_RTC->DOY = dateTime->tm_yday;
}

void RTC_SetSeconds(const time_t seconds){
	struct tm time;
	localtime_r(&seconds, &time);		//converts seconds since to struct
	time.tm_isdst = 1;
	RTC_SetValue(&time);
}

time_t RTC_GetSeconds(){
	struct tm time;
	RTC_GetValue(&time);		//gets current time
	return mktime(&time);	//converts to seconds since...
}
