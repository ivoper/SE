/*
 * spi.c
 *
 *  Created on: 13/11/2018
 *      Author: A42172
 */
#include "spi.h"
#include "LPC17xx.h"
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#define BIT_IN_PCONP 21				//ssp0 in pconp
#define MASK_IN_PCLKSEL1 0xC00		//ssp0 in pclksel1
#define MASK_IN_PINSEL0 	0xC0000000	//11000...
#define VALUE_TO_PINSEL0 	0x80000000	//10000...
#define MASK_IN_PINSEL1 	0x3C		//111100
#define VALUE_TO_PINSEL1 	0x28		//101000

#define TNF_BIT		0x02	//Transmit FIFO not full
#define RNE_BIT		0x04	//Recive FIFO not empty
#define TFE_BIT		0x01	//Transmit FIFO empty
#define RFF_BIT		0x08	//Recive FIFO full
#define BSY_BIT		0x10

void SPI_Init(){
	LPC_SC->PCONP |= 1<<BIT_IN_PCONP;								//asserts 1, ssp0 on
	LPC_SC->PCLKSEL1 &= ~(MASK_IN_PCLKSEL1);							//clk/4

	//selects function for ssp0
	unsigned int insert = LPC_PINCON->PINSEL0 & ~(MASK_IN_PINSEL0);		//bits to be modified to 0
	insert |= VALUE_TO_PINSEL0;											//or with value to bits to be modified
	LPC_PINCON->PINSEL0 = insert;							//value unchanged exept the right bits
	insert = LPC_PINCON->PINSEL1 & ~(MASK_IN_PINSEL1);		//bits to be modified to 0
	insert |= VALUE_TO_PINSEL1;								//or with value to bits to be modified
	LPC_PINCON->PINSEL1 = insert;							//value unchanged exept the right bits

	//interupts ?
}

//SystemCoreClock/frequency has to be an even number between 2 and 254, or clock might be faster than expected
void SPI_ConfigTransfer(int frequency, int bitData, int mode){
	unsigned int divider = (SystemCoreClock/4)/frequency;
	if (divider<2 || divider>254){
		//printf("frequencia nao suportada");
		return;
	}
	if (bitData<4 || bitData>16){
		//printf("numero bitData nao suportado");
		return;
	}
	if (mode<0 || mode>3){
		//printf("mode nao suportado");
		return;
	}
	LPC_SSP0->CPSR = divider;		//clock prescale
	unsigned int ctrl = (bitData - 1);	//control register code is less one, ex. 4 = 11
	ctrl |= mode<<6;	//Frame format bit 4..5(00=spi), (6 and 7 mode) 8..15 serial clock rate	(é para deixar a 0?)
	LPC_SSP0->CR0 = ctrl;
	LPC_SSP0->CR1 = 0x0;		//ssp disable, master mode

	LPC_SSP0->CR1 = 0x2;		//ssp enable
}

//WARNING: spi assumes bit used for CS is configured correctly by the user
void SPI_BeginTransfer(int csBit){
	LPC_GPIO0->FIOCLR = csBit;
}

void SPI_EndTransfer(int csBit){
	LPC_GPIO0->FIOSET = csBit;
}

void SPI_Transfer(unsigned short *txBuffer, unsigned short *rxBuffer, int length){
	//taskENTER_CRITICAL();
	if(length == 0)
		return;
	unsigned short in;
	unsigned short out;
	if(!txBuffer){
		out = 0;
	}else{
		out = *txBuffer++;
	}
	LPC_SSP0->DR = out;
	while(--length > 0){
		if(txBuffer){
			out = *txBuffer++;
		}
		while(LPC_SSP0->SR & BSY_BIT);
		in = LPC_SSP0->DR;
		LPC_SSP0->DR = out;
		if(rxBuffer){
			*rxBuffer++ = in;
		}
	}
	while(LPC_SSP0->SR & BSY_BIT);
	in = LPC_SSP0->DR;
	if(rxBuffer){
		*rxBuffer = in;
	}
	//taskEXIT_CRITICAL();
}
