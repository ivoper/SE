/*
 * wifi.c
 *
 *  Created on: 20/05/2019
 *      Author: Ivo Pereira
 */

#include "wifi.h"
#include "LPC17xx.h"
#include "uart.h"
#include "waitos.h"
#include <string.h>

#define GPIO_RST_PIN	21

#define UART_BAUD		115200

#define CR_LF_SIZE		2
#define CR				13
#define LF				10

void WIFI_Init(){
	UART_Init(UART_BAUD);
	LPC_GPIO0->FIOCLR = 1<<GPIO_RST_PIN;
	LPC_GPIO0->FIOSET = 1<<GPIO_RST_PIN;
	WAITOS_Ms(2000);
	ResetBuffers();
}

void clearBuffer(unsigned char *buffer){
	for(int i = 0; i<512; ++i){
		buffer[i] = 0;
	}
}

unsigned nDigits(const unsigned n) {
    if (n < 10) return 1;
    return 1 + nDigits(n / 10);
}

bool WIFI_Cmd(unsigned char* cmd, unsigned char* response, unsigned int *length, bool waitOK){
	unsigned int size = strlen((char *)cmd);
	unsigned char command[size + CR_LF_SIZE];
	unsigned char buffer[512];
	strcpy((char *)command, (char *)cmd);
	command[size] = CR;
	command[size + 1] = LF;
	bool read;
	read = UART_Send(command, size + CR_LF_SIZE);
	WAITOS_Ms(800);
	clearBuffer(buffer);		//clearing buffer between send and recieve to use wait time
	read = UART_Recieve(buffer, size + CR_LF_SIZE, true);
	for (int i = 0; i<512; ++i){
		if(buffer[i] == 0){
			response[i] = buffer[i];
			*length = i;
			return true;
		}
		response[i] = buffer[i];
	}
	return false;	//exceeded buffer size
}

bool WIFI_Data(unsigned char* data, unsigned int size, unsigned char* response, unsigned int *length, bool waitOK){
	unsigned char buffer[512];
	bool read;
	read = UART_Send(data, size);
	WAITOS_Ms(500);
	unsigned int digits = nDigits(size);
	unsigned int answerLength = 34 + (digits*2) + size;
	clearBuffer(buffer);		//clearing buffer between send and recieve to use wait time
	read = UART_Recieve(buffer, answerLength - 1, true);
	for (int i = 0; i<answerLength; ++i){
		response[i] = buffer[i];
	}
	*length = answerLength;
	return true;	//exceeded buffer size
}
