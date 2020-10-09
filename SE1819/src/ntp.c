/*
 * ntp.c
 *
 *  Created on: 14/07/2019
 *      Author: Ivo Pereira
 */

#include "ntp.h"
#include "LPC17xx.h"
#include "esp.h"

#define SERVER	"pool.ntp.org"
#define PORT	123
#define NTP_PACKET_SIZE		48

unsigned long NTP_GetTime(){
	unsigned char resp[128] = {0};
	unsigned int length;
	if (!ESP_StartTransfer("UDP", SERVER, PORT, resp, &length)){
		return 0;
	}
	char buffer[NTP_PACKET_SIZE] = {0};
	buffer[0] = 0b11100011;
	buffer[1] = 0;
	buffer[2] = 6;
	buffer[3] = 0xEC;
	if(ESP_Transfer(buffer, NTP_PACKET_SIZE, resp, &length)){
		char str[8];
		char* packet;
		sprintf(str, "+IPD,%d:", NTP_PACKET_SIZE);
		packet = strstr(resp, str);
		if(packet != 0){
			packet += 8;		//+IPD...
			ESP_CloseTransfer(resp, &length);
			return ((packet[40] << 24) | (packet[41] << 16) | (packet[42] << 8) | (packet[43]));
		}
	}
	ESP_CloseTransfer(resp, &length);
	return 0;
}
