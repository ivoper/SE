/*
 * lcd.c
 *
 *  Created on: 23/10/2018
 *      Author: A42172
 */

#include <stdarg.h>
#include <stdio.h>
#include "LPC17xx.h"
#include "lcd.h"
#include "wait.h"

#define LINE_0_START_ADDRESS 0x00
#define LINE_1_START_ADDRESS 0x40
#define CLEAR_DISPLAY_COMMAND 0x01

void WriteHalf(unsigned int bits, unsigned int rs){
	if (rs){
		LPC_GPIO2->FIOSET0 = 0x10;
	}
	else{
		LPC_GPIO2->FIOCLR0 = 0x10;
	}
	LPC_GPIO2->FIOSET0 = 0x20;		//enable 1
	LPC_GPIO2->FIOMASK0 = 0xF0;		//using only first 4 bits
	LPC_GPIO2->FIOPIN0 = bits;		//sends bits
	LPC_GPIO2->FIOMASK0 = 0x00;		//reset mask
	LPC_GPIO2->FIOCLR0 = 0x20;		//enable 0
}

static void WriteByte(unsigned int bits, unsigned int rs){
	unsigned int firstHalf = bits & 0x0F;		//split byte in two
	unsigned int secondHalf = bits & 0xF0;
	secondHalf = secondHalf >> 4;
	WAIT_Ms(2);
	WriteHalf(secondHalf, rs);					//send high part first
	WriteHalf(firstHalf, rs);
}

static void WriteCmd(unsigned int cmd){
	WriteByte(cmd, 0);
}

static void WriteData(unsigned int data){
	WriteByte(data, 1);
}

void LCDText_Init(void){
	LPC_GPIO2->FIODIR0 |= 0x3F;		//gpio2[0] - gpio2[5] to output

	WAIT_Ms(15);
	WriteHalf(0x03, 0);
	WAIT_Ms(5);
	WriteHalf(0x03, 0);
	WAIT_Ms(1);
	WriteHalf(0x03, 0);
	WriteHalf(0x02, 0);
	WriteCmd(0x28);
	WriteCmd(0x08);
	WriteCmd(0x01);
	WriteCmd(0x06);
	WriteCmd(0x0F);			//display, cursor on
}

void LCDText_WriteChar(char ch){
	WriteData((unsigned int)ch);
}

void LCDText_WriteString(char* str){
	int i = 0;
	while(str[i] != '\0'){
		WriteData((unsigned int)str[i]);
		++i;
	}
}

void LCDText_Locate(unsigned int row, unsigned int column){
	if (row > 1){
		printf("numero de linha errado");
	}
	if(column > 39){
		printf("numero de coluna errado");
	}
	unsigned int cmd;
	if(row == 0){
		cmd = LINE_0_START_ADDRESS;
	}else if(row == 1){
		cmd = LINE_1_START_ADDRESS;
	}
	cmd += column;
	cmd |= 0x80;		//assures command used is set DDRAM address
	WriteCmd(cmd);
}

void LCDText_Clear(){
	WriteCmd(CLEAR_DISPLAY_COMMAND);
}

void LCDText_Printf(char *fmt, ...){
	char str[32];
	va_list argptr;
	va_start(argptr,fmt);
	vsprintf(str, fmt, argptr);
	va_end(argptr);
	LCDText_WriteString(str);
}
