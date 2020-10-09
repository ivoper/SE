/*
 * keypad.c
 *
 *  Created on: 09/10/2018
 *      Author: A42172
 */

#include <string.h>
#include "keypad.h"
#include "wait.h"
#include "LPC17xx.h"

#define BITS_TO_PINMODE (0x3f << 14)	//bits in correct position to zero in pinmode

#define INPUT_MASK	(0x7 << 23)	// input pins for keyboard
#define OUTPUT_MASK	(0xF << 6)	// output pins for keyboard
#define SHIFT_TO_OUTPUT 6
#define SHIFT_TO_INPUT 23

#define LAYOUT_SIZE 		12
#define VAL_TO_XOR			0x07	//seek explanation

int DefinedLayout[LAYOUT_SIZE];

/*
 * verifies if any of the input pins (3 column bits) is different than 1;
 * receiving 0 in any of the bits means a key in said column was pressed
 */
int KeyCaught(void){
	if ((LPC_GPIO0->FIOPIN & INPUT_MASK) != INPUT_MASK){
			return 1;
		}
		return 0;
}

/*
 * verifies if combination (* and #) is being pressed
 */
int CheckCombination(){
	WAIT_Ms(50);
	unsigned int x = (LPC_GPIO0->FIOPIN & INPUT_MASK)>>SHIFT_TO_INPUT;	//input value aligned
	return x == 2;		//010 -> * and # pressed
}

void KEYPAD_Init(int layout[12]){
	LPC_PINCON->PINMODE1 &= ~(BITS_TO_PINMODE);		//assures input bits are in pull up mode
	LPC_GPIO0->FIODIR &= ~(INPUT_MASK);		//bits for input to 0
	LPC_GPIO0->FIODIR |= OUTPUT_MASK;		//bits for output to 1
	memcpy(DefinedLayout, layout, LAYOUT_SIZE * sizeof(int));
}

int KEYPAD_Hit(void){
	LPC_GPIO0->FIOCLR = OUTPUT_MASK;		//send 0 to all keyboard lines
	return KeyCaught();
}

int KEYPAD_Read(void){
	int line = 0;		//line goes from 0 to 3 (length 4); column goes from 1 to 3 (length 3)
	while(1){
		LPC_GPIO0->FIOSETL = OUTPUT_MASK;	//output bits to 1
		LPC_GPIO0->FIOCLRL = 1<<(SHIFT_TO_OUTPUT + line);	//line output bit to 0
		if(KeyCaught()){
			int col = (LPC_GPIO0->FIOPIN & INPUT_MASK)>>SHIFT_TO_INPUT;	//input bits(column bits) to col
			if (col != NUMBER_OF_COLUMNS)			//if key is pressed in last column then the col value is correct
				col ^= VAL_TO_XOR;					//if key is pressed in other columns this xor puts correct value in col
			if (line == NUMBER_OF_LINES - 1){		//combination supported in last line(row) of keypad
				if (CheckCombination()){
					return COMBINATION_CODE;
				}
			}
			if (col > NUMBER_OF_COLUMNS){
				return KEY_COMBINATION_UNSUPPORTED_CODE;
			}
			return DefinedLayout[col + (line*(NUMBER_OF_LINES -1)) - 1];
		}
		if (++line == NUMBER_OF_LINES){
			line = 0;
		}
	}
}
