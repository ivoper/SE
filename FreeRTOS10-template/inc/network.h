/*
 * network.h
 *
 *  Created on: 15/07/2019
 *      Author: Ivo Pereira
 */

#ifndef INC_NETWORK_H_
#define INC_NETWORK_H_

#include <stdbool.h>
#include <time.h>

bool ConnectToAP(const char* ssid, const char* pass);

time_t GetCurrentTime();

#endif /* INC_NETWORK_H_ */
