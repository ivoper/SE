/*
 * wait.c
 *
 *  Created on: 16/10/2018
 *      Author: A42172
 */
#include "LPC17xx.h"

volatile uint32_t milli;

void SysTick_Handler(){
	milli++;		//count milliseconds in each sysTick interrupt
}

int WAIT_Init(void){
	//configure sysTick for each millisecond
	return SysTick_Config(SystemCoreClock/1000);	//SysCoreClock = nTicks per second, /1000 -> nTicks per millisecond
}

void WAIT_Ms(uint32_t m){
	uint32_t start = milli;
	if(start > UINT32_MAX - m){		//if true, milli would go around from max val to 0 during wait
		start = milli = 0;
	}
	while(milli < start+m)			//sleep while m milliseconds haven't passed since start
		;
}
