/*
 * mfrc.c
 *
 *  Created on: 20/11/2018
 *      Author: A42172
 */

#include "mfrc.h"
#include "spi.h"
//#include "wait.h"
#include "waitos.h"
#include <stdlib.h>

#define IRQ_BIT		1			//in
#define RST_BIT		(1<<1)		//out
#define CS_BIT		(1<<16)		//out, bit 16

#define GPIO_PINSEL0_BITS 			0x0F
#define GPIO_PINSEL1_BITS 			0x03	//0011 for CS
#define GPIO_INPUT_PINMODE0_BITS 	0x0C

#define TRANSF_FREQ 		6250000	//1 000 000
#define TRANSF_BIT_SIZE 	8
#define TRANSF_MODE			0			//data stable on raising edge and cs remains low in continuous transmission;

void MFRC522_PreInit(void){
	LPC_PINCON->PINSEL0 &= ~(GPIO_PINSEL0_BITS);		//assures function GPIO for irq and rst bits
	LPC_PINCON->PINSEL1 &= ~(GPIO_PINSEL1_BITS);		//assures function GPIO for ss
	LPC_PINCON->PINMODE0 &= ~(GPIO_INPUT_PINMODE0_BITS);		//assures input bits are in pull up mode
	LPC_GPIO0->FIODIR &= ~(IRQ_BIT);		//irq input
	LPC_GPIO0->FIODIR |= (RST_BIT | CS_BIT);			//rst and ss output
	SPI_Init();
	SPI_ConfigTransfer(TRANSF_FREQ, TRANSF_BIT_SIZE, TRANSF_MODE);
}
/* Realiza as iniciações necessárias para a biblioteca (Configuração do
barramento e pinos). */

void MFRC522_Reset(bool assert){
	if (!assert){
		LPC_GPIO0->FIOSET = RST_BIT;
	}
	else{
		LPC_GPIO0->FIOCLR = RST_BIT;
	}
}
/* Manipula o pino de reset do RC522 de acordo com o parâmetro. */

void MFRC522_Select(bool assert){
	if (assert){
		SPI_BeginTransfer(CS_BIT);
	}
	else{
		SPI_EndTransfer(CS_BIT);
	}
}
/* Manipula o pino de chip select do RC522 de acordo com o parâmetro. */

uint8_t MFRC522_Transfer(uint8_t value){
	unsigned short txBuffer = (unsigned short)value;
	unsigned short rxBuffer;
	SPI_Transfer(&txBuffer, &rxBuffer, 1);
	return (uint8_t)rxBuffer;
}
/* Realiza a transmissão/receção de byte. */

void MFRC522_WaitMs(uint32_t millis){
	WAITOS_Ms(millis);
	//WAIT_Ms(millis);
}
/* Espera, em milissegundos, o valor do parâmetro. */
