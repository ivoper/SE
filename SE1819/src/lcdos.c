/*
 * lcdos.c
 *
 *  Created on: 06/06/2019
 *      Author: Ivo Pereira
 */

#include "lcdos.h"
#include "waitos.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define LINE_0_START_ADDRESS	0x00
#define LINE_1_START_ADDRESS	0x40
#define CLEAR_DISPLAY_COMMAND	0x01
#define LINE_LENGTH				16

QueueHandle_t QueueLcd;

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
	WAITOS_Ms(2);
	WriteHalf(secondHalf, rs);					//send high part first
	WriteHalf(firstHalf, rs);
}

static void WriteCmd(unsigned int cmd){
	WriteByte(cmd, 0);
}

static void WriteData(unsigned int data){
	WriteByte(data, 1);
}

void WriteChar(char ch){
	WriteData((unsigned int)ch);
}

void WriteString(char* str){
	int i = 0;
	while(str[i] != '\0'){
		WriteData((unsigned int)str[i]);
		++i;
	}
}

void Locate(unsigned int row, unsigned int column){
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

void Clear(){
	WriteCmd(CLEAR_DISPLAY_COMMAND);
}

void Print(char *str){
	size_t l = strlen(str);
	char buffer[LINE_LENGTH];
	if(l<=LINE_LENGTH){
		WriteString(str);
	}else{
		memcpy(buffer, str, LINE_LENGTH);	//primeira linha para buffer
		WriteString(buffer);
		WriteString(str+LINE_LENGTH);		//segunda linha
	}
}

//called by lcd task
void LCDOS_Init(QueueHandle_t queue){
	LPC_GPIO2->FIODIR0 |= 0x3F;		//gpio2[0] - gpio2[5] to output
	QueueLcd = queue;
	WAITOS_Ms(15);
	WriteHalf(0x03, 0);
	WAITOS_Ms(5);
	WriteHalf(0x03, 0);
	WAITOS_Ms(1);
	WriteHalf(0x03, 0);
	WriteHalf(0x02, 0);
	WriteCmd(0x28);
	WriteCmd(0x08);
	WriteCmd(0x01);
	WriteCmd(0x06);
	WriteCmd(0x0F);			//display, cursor on
}

void LCDOS_ProcessCmd(){
	char cmd[CMD_SIZE];
	char key;
	xQueueReceive(QueueLcd, cmd, portMAX_DELAY);
	memcpy(&key, cmd, 1);
	switch(key){
		case 'L':
			Locate((unsigned int)cmd[1], (unsigned int)cmd[2]);
			break;
		case 'C':
			Clear();
			break;
		case 'P':
			Print(&cmd[1]);
			break;
	}
}

//called from control/app task
int LCDOS_Locate(unsigned int row, unsigned int column){
	if (row > 1){
		return ILLEGAL_ROW;
	}
	if(column > 39){
		return ILLEGAL_COL;
	}
	char cmd[33] = {'L', (char)row, (char)column};
	if(xQueueSendToBack(QueueLcd, cmd, 0) == pdPASS){
		return 1;
	}else{
		return 0;
	}
}

int LCDOS_Clear(void){
	char cmd[33] = {'C'};
	if(xQueueSendToBack(QueueLcd, cmd, 0) == pdPASS){
		return 1;
	}else{
		return 0;
	}
}

int LCDOS_Printf(char *fmt, ...){
	char cmd[33] = {'P'};
	va_list argptr;
	va_start(argptr,fmt);
	vsprintf(&cmd[1], fmt, argptr);
	va_end(argptr);
	if(xQueueSendToBack(QueueLcd, cmd, 0) == pdPASS){
		return 1;
	}else{
		return 0;
	}
}
