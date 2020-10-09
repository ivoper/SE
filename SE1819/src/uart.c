/*
 * uart.c
 *
 *  Created on: 09/04/2019
 *      Author: Ivo Pereira
 */
#include "uart.h"
#include "rb.h"
#include "LPC17xx.h"

#define UART0_PCON_BIT			3
#define UART0_PCLK_BIT			6
#define UART0_PINSEL_MASK		0xF0
#define UART0_PINSEL_VAL		0x50

//when changing one of these, update the other
#define PCLK_VAL		0
#define PCLK_DIV		4

#define RLS_ID		0x03
#define RDA_ID		0x02
#define CTI_ID		0x06
#define THRE_ID		0x01

#define TIMEOUT_HUGE	10000000
#define TIMEOUT_SMALL	100

static bool txEmpty;
static RINGBUFF_DSC TxBuff, RxBuff;

uint32_t GetUartDL(uint32_t baudrate){
	return (100000000/PCLK_DIV)/(16*baudrate);		//Divisor Latch = PCLK/16*baudrate
}

void ResetBuffers(){
	RB_Reset(&TxBuff);
	RB_Reset(&RxBuff);
}

void UART0_IRQHandler(){
	uint32_t status = LPC_UART0->IIR;
	uint32_t error = 0;
	unsigned int fordebug;
	while(!(status & 0x1)){
		switch((status >> 1) & 0x7){
			case RLS_ID:
				error = LPC_UART0->LSR;
				//printf("UART RLS triggered, error code: 0x%2x\n", error);
				break;
			case RDA_ID:
			case CTI_ID:
				if(!RB_Full(&RxBuff)){
					RB_Push(&RxBuff, LPC_UART0->RBR);
				}else{
					error = -1;
				}
				break;
			case THRE_ID:
				if(RB_Empty(&TxBuff)){
					txEmpty = true;
				}else{
					fordebug = RB_Pop(&TxBuff);
					LPC_UART0->THR = fordebug;
				}
				break;
			default:
				while(1);
		}
		status = LPC_UART0->IIR;
	}
}

void UART_Init(unsigned int baudrate){
	LPC_PINCON->PINSEL0 &= ~(UART0_PINSEL_MASK);
	LPC_PINCON->PINSEL0 |= UART0_PINSEL_VAL;
	LPC_SC->PCONP |= (1<<UART0_PCON_BIT);
	LPC_SC->PCLKSEL0 &= ~(3 << UART0_PCLK_BIT);
	LPC_SC->PCLKSEL0 |= (PCLK_VAL << UART0_PCLK_BIT);
	LPC_UART0->LCR = 0x80;	//access to baudrate
	unsigned int dl = GetUartDL(baudrate);
	LPC_UART0->DLM = dl >> 8;		//baudrate high
	LPC_UART0->DLL = dl;			//baudrate low
	LPC_UART0->LCR = 0x03;			//8-bit char
	LPC_UART0->FCR = 1;				//enable FIFO
	LPC_UART0->IER = 0x07;			//enable interrupts mask
	NVIC_SetPriority(UART0_IRQn, 5);
	NVIC_EnableIRQ(UART0_IRQn);
	txEmpty = 1;
	ResetBuffers();
}

bool UART_Send(unsigned char *buffer, unsigned int size){
	for(int i = 0; i<size; ){
		if(!RB_Full(&TxBuff)){		//e se não couber tudo à primeira??
			RB_Push(&TxBuff, buffer[i++]);
		}else{
			LPC_UART0->THR = RB_Pop(&TxBuff);	//full then has values
			__WFI();
		}
	}
	if(txEmpty){
		if(!RB_Empty(&TxBuff)){
			txEmpty = false;
			LPC_UART0->THR = RB_Pop(&TxBuff);
		}
	}
	while(!RB_Empty(&TxBuff)){
		__WFI();
	}
	return true;
}

bool UART_Recieve(unsigned char *buffer, unsigned int size, bool waitOK){
	unsigned int timeout = 0;
	bool ok = !waitOK;
	for(int i=0; i<512; ){
		if(!RB_Empty(&RxBuff)){
			buffer[i++] = RB_Pop(&RxBuff);
			timeout = 0;
			if(!ok && buffer[i-1] == 'K'){
				if(buffer[i-2] == 'O')
					ok = true;
			}
			continue;
		}
		if(i <= size || !ok){			//if response hasn't start
			if (++timeout > TIMEOUT_HUGE){
				return false;
			}
		}else{				//in middle of response, timeout faster
			if (++timeout > TIMEOUT_SMALL){
				return true;
			}
		}
	}
	return false;	//answer too big
}
