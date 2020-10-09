/*
 * i2c_status.h
 *
 *  Created on: 12/03/2019
 *      Author: Ivo Pereira
 */

#ifndef I2C_STATUS_H_
#define I2C_STATUS_H_

//Master
#define STATE_START				0x08
#define STATE_REPEATED_START	0x10
#define STATE_LOST				0x38

//Master Transmitter
#define STATE_TRAN_ADDR_ACK			0x18
#define STATE_TRAN_ADDR_NACK		0x20
#define STATE_TRAN_DATA_ACK			0x28
#define STATE_TRAN_DATA_NACK		0x30

//Master Receiver
#define STATE_RCV_ADDR_ACK			0x40
#define STATE_RCV_ADDR_NACK			0x48
#define STATE_RCV_DATA_ACK			0x50
#define STATE_RCV_DATA_NACK			0x58

//Miscellaneous States
#define STATE_NOTHING			0xF8
#define STATE_BUS_ERROR			0x00

#endif /* I2C_STATUS_H_ */
