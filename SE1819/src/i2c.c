/*
 * i2c.c
 *
 *  Created on: 26/02/2019
 *      Author: Ivo Pereira
 */

#include "i2c.h"
#include "i2c_status.h"
#include <stdbool.h>
#include "LPC17xx.h"

#define I2CCON_ENABLE	0x40		//i2c enable
#define I2CCON_START	0x20		//i2c start
#define I2CCON_STOP		0x10		//i2c stop
#define I2CCON_INT		0x08		//i2c SI
#define I2CCON_ACK 		0x04		//i2c AA

#define BIT_IN_PCONP 		26
#define MASK_IN_PCLKSEL1		0x300000

#define ISER0_ENABLE		0x1000

#define MASK_IN_PINSEL		0xF00000
#define VAL_FOR_PINSEL		0xA00000

#define QUEUE_LENGTH		300

struct Queue{
	unsigned char byte[QUEUE_LENGTH];
	unsigned int outIx;		//used to read from queue
	unsigned int inIx;		//used to write for queue
	unsigned int nbytes;
	bool toStop;
	bool busy;
};

struct Queue queue;

void I2C2_IRQHandler(){
	short status = LPC_I2C2->I2STAT;
	switch(status){
		case STATE_NOTHING:
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;

		case STATE_START:
			LPC_I2C2->I2DAT = queue.byte[queue.outIx++];	//set address and write/read bit
			LPC_I2C2->I2CONCLR = I2CCON_START | I2CCON_INT;
			break;

		case STATE_REPEATED_START:
			LPC_I2C2->I2DAT = queue.byte[queue.outIx++];	//set address and write/read bit
			LPC_I2C2->I2CONCLR = I2CCON_START | I2CCON_INT;
			break;

			//****TRANSMITTER****
		case STATE_TRAN_ADDR_ACK:
			if(queue.outIx >= queue.inIx){	//interruption has to wait, no data to write
				NVIC_DisableIRQ(I2C2_IRQn);
				break;
			}
			LPC_I2C2->I2DAT = queue.byte[queue.outIx++];	//write next byte
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;
		case STATE_TRAN_ADDR_NACK:
			LPC_I2C2->I2CONSET = I2CCON_START;	//restart
			queue.outIx--;						//pointer to address again
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;
		case STATE_TRAN_DATA_ACK:
			if(queue.outIx >= queue.inIx){	//interruption has to wait, no data to write
				if(queue.outIx < queue.nbytes){
					NVIC_DisableIRQ(I2C2_IRQn);
					break;
				}
				//transfer complete
				if(queue.toStop){			//if end and is to stop do clr int and set stop, otherwise do neither
					LPC_I2C2->I2CONSET = I2CCON_STOP;
					LPC_I2C2->I2CONCLR = I2CCON_INT;
					break;
				} else{
					NVIC_DisableIRQ(I2C2_IRQn);
					break;
				}
			}else{
				unsigned char b = queue.byte[queue.outIx++];
				LPC_I2C2->I2DAT = b;	//write next byte
			}
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;
		case STATE_TRAN_DATA_NACK:	//?????
			if (queue.toStop){
				LPC_I2C2->I2CONSET = I2CCON_STOP;	//stop
			}
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;

			//****RECEIVER****
		case STATE_RCV_ADDR_ACK:
			LPC_I2C2->I2CONSET = I2CCON_ACK;	//to acknowledge next data received
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;
		case STATE_RCV_ADDR_NACK:
			LPC_I2C2->I2CONSET = I2CCON_START;	//restart
			queue.outIx--;						//pointer to address again
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;
		case STATE_RCV_DATA_ACK:
			queue.byte[queue.inIx++] = LPC_I2C2->I2DAT;	//read next byte
			if((queue.inIx == (queue.nbytes -1))){		//only last byte missing
				LPC_I2C2->I2CONCLR = I2CCON_ACK;
			}
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;
		case STATE_RCV_DATA_NACK:
			queue.byte[queue.inIx++] = LPC_I2C2->I2DAT;	//read next byte
			LPC_I2C2->I2CONSET = I2CCON_STOP;	//stop
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;

		case STATE_LOST:
			//?
			LPC_I2C2->I2CONCLR = I2CCON_INT;
			break;
	}
}

void I2C_Init(short clh, short cll){
	LPC_SC->PCONP |= 1<<BIT_IN_PCONP;								//asserts 1, i2c2 on
	LPC_SC->PCLKSEL1 &= ~(MASK_IN_PCLKSEL1) | (1<<20);						//pclk = clk

	unsigned int insert = LPC_PINCON->PINSEL0 & ~(MASK_IN_PINSEL);		//bits to be modified to 0
	insert |= VAL_FOR_PINSEL;											//or with value to bits to be modified
	LPC_PINCON->PINSEL0 = insert;							//value unchanged exept the right bits

	LPC_I2C2->I2SCLH = clh;
	LPC_I2C2->I2SCLL = cll;

	LPC_I2C2->I2CONCLR = I2CCON_START | I2CCON_ACK | I2CCON_ENABLE | I2CCON_INT |I2CCON_STOP;	//peace of mind
	LPC_I2C2->I2CONSET = I2CCON_ENABLE;		//enable I2CON

	//enable interrupts
	NVIC_SetPriority(I2C2_IRQn, 5);
	NVIC_EnableIRQ(I2C2_IRQn);
}

int I2C_Transfer(unsigned char SLA, unsigned int length, unsigned char* data, bool toStop){
	//check if available and prepare queue
	if(queue.busy){
		return UNSUCCESSFUL;
	}
	queue.busy = true;
	queue.inIx = 0;
	queue.outIx = 0;
	queue.toStop = toStop;
	queue.nbytes = length + 1;		//+1 for addr
	queue.byte[queue.inIx++] = SLA;	//admitting address is already shifted left; 0 to write
	//start transfer
	LPC_I2C2->I2CONSET = I2CCON_START;
	for(int i = 0; i<length; ++i){
		queue.byte[queue.inIx++] = data[i];
		if(i%2 == 0)							//to give advantage
			NVIC_EnableIRQ(I2C2_IRQn);			//if previous interrupt could not be resolved
	}
	while(queue.outIx != queue.nbytes){		//transfer not complete
		NVIC_EnableIRQ(I2C2_IRQn);
	}
	NVIC_EnableIRQ(I2C2_IRQn);
	queue.busy = false;		//free queue
	if(toStop){
		return SUCCESSFUL_STOPED;
	}
	else{
		return SUCCESSFUL_HALTED;
	}
}

int I2C_Receive(unsigned char SLA, unsigned int length, unsigned char* data){
	if(queue.busy){
		return UNSUCCESSFUL;
	}
	queue.busy = true;
	queue.inIx = 0;
	queue.outIx = 0;
	if(length == 0){
		length = QUEUE_LENGTH -1;
	}
	queue.toStop = true;
	queue.nbytes = length + 1;
	queue.byte[queue.inIx++] = SLA | 1;	//admitting address is already shifted left; 1 to read
	LPC_I2C2->I2CONSET = I2CCON_START;
	LPC_I2C2->I2CONCLR = I2CCON_INT;
	NVIC_EnableIRQ(I2C2_IRQn);
	for(int i = 0; i<length; ++i){
		if((queue.outIx >= queue.inIx) || (queue.inIx == 1)){	//no data in queue to read
			--i;
			NVIC_EnableIRQ(I2C2_IRQn);			//if previous interrupt could not be resolved
			continue;
		}
		data[i] = queue.byte[queue.outIx++];
	}
	queue.busy = false;		//free queue
	if(queue.outIx != queue.inIx){	//interrupted transference
		return UNSUCCESSFUL;
	}
	return SUCCESSFUL_STOPED;
}
