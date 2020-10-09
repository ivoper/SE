/*
===============================================================================
 Name        : Test_Lab.c
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
#include "e2prom.h"
#include "rb.h"
#include "uart.h"
#include "wait.h"
#include "wifi.h"
#include "esp.h"
#include "ntp.h"
#include "time.h"

#define E2P_ADDR	0x0100

#define BUFFER_SIZE	128
#define MASTER_CARD_ID 0x1F016D86	//TODO garantir que ta certo

struct record{
	unsigned int cardId;
	struct tm date;
	char valid;
};

struct control{
	unsigned short cardsIx;
	unsigned short recordsIx;
	unsigned int maintenanceCards[3];
};

void test_e2p_mem(){
	E2P_Init();
	bool v[7] = {false};

	//ctrl
	struct control ctrl = {.maintenanceCards[0] = MASTER_CARD_ID};
	//E2P_Write(0, sizeof(struct control), (unsigned char *)&ctrl);
	//records
	struct record record = {};
	struct tm date = {};
	/*
	for(int i = 0; i<6; ++i){
		date.tm_hour = i;
		record.cardId = 100000 * i;
		record.date = date;
		record.valid = (i % 2) == 0;
		v[i] = E2P_Write(0x100 + (sizeof(struct record)*i), sizeof(struct record), (unsigned char *)&record);
	}

	unsigned char write[300] = {0};
	for(int i = 0; i<6; ++i){
		v[i] = E2P_Write(256+(44*i), 44, write+(44*i));
	}
*/
	//unsigned char write[300] = {0};
	//v[0] = E2P_Write(256+200, 44, write);
	for(int i = 0; i<6; ++i){
		v[6] = E2P_Read(0x100*(sizeof(struct record)*i), sizeof(struct record), (unsigned char *)&record);
	}
}

void test_i2c_rw(){
	unsigned char array[44];
    for(int i = 0; i<44; ++i){
    	array[i] = 44 - i;
    }
	unsigned char result[100];
	E2P_Init();
    E2P_Write(E2P_ADDR, 44, array);
    E2P_Read(E2P_ADDR, 100, result);
    for(int i = 0; i<8; ++i){
    	printf("%d", result[i]);
    }
    printf("\n");
}

void test_i2c_ro(){
	unsigned char result[8];
	E2P_Init();
    E2P_Read(0x0102, 8, result);
    for(int i = 0; i<8; ++i){
    	printf("%d", result[i]);
    }
    printf("\n");
}

void test_rb(){
	RINGBUFF_DSC rb;
	RB_Reset(&rb);
	for(int i = 0; i<260; ++i){
		printf("%d", rb.buffer[i]);
		if(!RB_Full(&rb))
		RB_Push(&rb, i);
		printf("%d\n", rb.buffer[i]);
	}
}

void test_uart(){
	WAIT_Init();
	unsigned char buffer[256];
	for (int i = 0; i<256; ++i){
		buffer[i] = i;
	}
	unsigned char result[256];
	UART_Init(115200);
	UART_Send(buffer, 256);
	UART_Recieve(result, 256, false);
	for(int i = 0; i<256; ++i){
		printf("%d\n", result[i]);
	}
}

void cleanBuffer(unsigned char *buffer){
	for(int i = 0; i<512; ++i){
		buffer[i] = 0;
	}
}

void test_wifi(){
	ESP_Init();
	unsigned char *ssid = (unsigned char *)"NOS-Sala";
	unsigned char *pass = (unsigned char *)"bruno1967";
	//unsigned char r[512];
	unsigned char a[BUFFER_SIZE];
	unsigned int length;
	/*
	if(ESP_Restart( a, &length)){
		for (int i=0; i<length; ){
			printf("%c", a[i++]);
		}
	}else{
		printf("command 1 not executed correctly, length is %d\n", length);
	}
	*/

	if(ESP_SetOperationMode(1, a, &length)){
		for (int i=0; i<length; ){
			printf("%c", a[i++]);
		}
	}else{
		printf("command 1 not executed correctly, length is %d\n", length);
	}

	if(ESP_ConnectToNetwork(ssid, pass, a, &length)){
		for (int i=0; i<length; ){
			printf("%c", a[i++]);
		}
	}else{
		printf("command 2 not executed correctly, length is %d\n", length);
	}

	if(ESP_SetDNS("8.8.4.4", a, &length)){
		for (int i=0; i<length; ){
			printf("%c", a[i++]);
		}
	}else{
		printf("command 3 not executed correctly, length is %d\n", length);
	}

	unsigned long secs = NTP_GetTime();
	secs -= 2208988800UL;		//1900 -> 1970
	secs += 60*60;				//timezone
	struct tm *t = localtime(&secs);
	t->tm_isdst = 1;
}

int main(void) {
	WAIT_Init();
	test_e2p_mem();
	//test_i2c_rw();
	//test_i2c_ro();
	//test_rb();
	//test_uart();
	//test_wifi();
    while(1);
}
