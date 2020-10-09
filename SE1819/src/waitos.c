/*
 * waitos.c
 *
 *  Created on: 04/06/2019
 *      Author: Ivo Pereira
 */

#include "waitos.h"

#include "FreeRTOS.h"
#include "task.h"

void WAITOS_Ms(unsigned int millis){
	vTaskDelay(millis);
}
