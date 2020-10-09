/*
 * e2prom.c
 *
 *  Created on: 19/03/2019
 *      Author: Ivo Pereira
 */
#include "i2c.h"
#include "e2prom.h"
#include "wait.h"
#include <string.h>

#define ADDRESS_BYTE	0xA0

#define HIGH_CYCLE		100
#define LOW_CYCLE		150

#define MAX_TRANSFER_BYTES		64
#define ADDITIONAL_ADDR_BYTES	2

void E2P_Init(){
	I2C_Init(HIGH_CYCLE, LOW_CYCLE);
}

int E2P_Write(unsigned short address, unsigned int n_bytes, unsigned char *buffer){
	if(n_bytes > MAX_TRANSFER_BYTES){
		return UNSUPPORTED_BYTE_NUMBER;
	}
	unsigned int length = n_bytes + ADDITIONAL_ADDR_BYTES;
	unsigned char array[length];
	array[0] = (char)(address>>8);
	array[1] = (char)address;
	for(int i = 2; i<length; ++i){
		array[i] = buffer[i-2];
	}
	int ret = I2C_Transfer(ADDRESS_BYTE, length, array, true);
	WAIT_Ms(5);
	return ret;
}

int E2P_Read(unsigned short address, unsigned int n_bytes, unsigned char *buffer){
	unsigned char addr[2];
	addr[0] = (char)(address>>8);
	addr[1] = (char)address;
	if(I2C_Transfer(ADDRESS_BYTE, ADDITIONAL_ADDR_BYTES, addr, false) != SUCCESSFUL_HALTED){
		return -1;
	}
	WAIT_Ms(5);
	return I2C_Receive(ADDRESS_BYTE, n_bytes, buffer);
}
