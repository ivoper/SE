/*
 * led.c
 *
 *  Created on: 14/10/2018
 *      Author: A42172
 */

#include "LPC17xx.h"
#include "led.h"

#define GPIO_PORT_DISTANCE 0x0020

LPC_GPIO_TypeDef* gpio;
int pinMask;

//does not support function selection; pin to be used must be already in gpio function
void LED_Init(int portId, int pinId, int state){
	gpio = (LPC_GPIO_TypeDef*)(LPC_GPIO_BASE + (GPIO_PORT_DISTANCE * portId));	//gets gpioPort address (pointer)
	pinMask = 1<<pinId;			//creates mask from pinId
	gpio->FIODIR |= pinMask;	//set pin to output
	if (state == 0){
		LED_Off();
	}
	else{
		LED_On();
	}
}

int LED_GetState(void){
	return gpio->FIOPIN & pinMask;
}

void LED_On(void){
	gpio->FIOSET = pinMask;
}

void LED_Off(void){
	gpio->FIOCLR = pinMask;
}
