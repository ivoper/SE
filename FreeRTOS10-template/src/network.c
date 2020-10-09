/*
 * network.c
 *
 *  Created on: 15/07/2019
 *      Author: Ivo Pereira
 */

#include "network.h"
#include "esp.h"
#include "ntp.h"

#define BUFFER_SIZE	128

bool ConnectToAP(const char* ssid, const char* pass){
	unsigned char a[BUFFER_SIZE];
	unsigned int length;

	if(!ESP_SetOperationMode(1, a, &length)){
		return false;
	}
	if(!ESP_ConnectToNetwork((unsigned char *)ssid, (unsigned char *)pass, a, &length)){
		return false;
	}
	if(!ESP_SetDNS("8.8.4.4", a, &length)){
		return false;
	}
	return true;
}

time_t GetCurrentTime(){
	unsigned long secs = NTP_GetTime();
	secs = secs - 2208988800UL;		//1900 -> 1970
	secs = secs + 60*60;				//timezone
	return (time_t)secs;
}
