/*
 * esp.c
 *
 *  Created on: 10/07/2019
 *      Author: Ivo Pereira
 */

#include "esp.h"
#include "wifi.h"
#include "math.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

unsigned numDigits(const unsigned n) {
    if (n < 10) return 1;
    return 1 + numDigits(n / 10);
}

void ESP_Init(){
	WIFI_Init();
}

int ESP_SetOperationMode(int mode, unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	char cmd[16];
	snprintf(cmd, 16, "AT+CWMODE_CUR=%d", mode);
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_QuitNetwork(unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	char cmd[9] = "AT+CWQAP";
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_ListAvailableNetworks(unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	char cmd[9] = "AT+CWLAP";
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_ConnectToNetwork(unsigned char* ssid, unsigned char* pass, unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	unsigned int acum = strlen(ssid) + strlen(pass) + 19;
	char cmd[acum];
	snprintf(cmd, acum, "AT+CWJAP_CUR=\"%s\",\"%s\"", ssid, pass);
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_GetIP(unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	char cmd[9] = "AT+CIFSR";
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_CheckIP(unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	char cmd[11] = "AT+CIPSTA?";
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_Ping(unsigned char* addr, unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	unsigned int acum = strlen(addr) + 11;
	char cmd[acum];
	snprintf(cmd, acum, "AT+PING=\"%s\"", addr);
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_SetDNS(unsigned char* dns, unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	unsigned int acum = strlen(dns) + 19;
	char cmd[acum];
	snprintf(cmd, acum, "AT+CIPDNS_CUR=1,\"%s\"", dns);
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_StartTransfer(unsigned char* type, unsigned char* addr, unsigned int port, unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	unsigned int acum = strlen(type) + strlen(addr) + numDigits(port) + 19;
	char cmd[acum];
	snprintf(cmd, acum, "AT+CIPSTART=\"%s\",\"%s\",%d", type, addr, port);
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_Transfer(unsigned char* data, unsigned int size, unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	unsigned int acum = numDigits(size) + 12;
	char cmd[acum];
	snprintf(cmd, acum, "AT+CIPSEND=%d", size);
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, false)){
		if(strstr(buffer, ">") != NULL){
			if(WIFI_Data(data, size, buffer, length, true)){
				memcpy(response, buffer, *length);
				return 1;
			}
		}
	}
	return 0;
}

int ESP_CloseTransfer(unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	char cmd[12] = "AT+CIPCLOSE";
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}

int ESP_Restart(unsigned char* response, unsigned int *length){
	unsigned char buffer[512];
	char cmd[7] = "AT+RST";
	if (WIFI_Cmd((unsigned char*)cmd, buffer, length, true)){
		memcpy(response, buffer, *length);
		return 1;
	}else{
		return 0;
	}
}
