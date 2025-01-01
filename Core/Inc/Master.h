/*
 * Master.h
 *
 *  Created on: Dec 7, 2024
 *      Author: ADMIN
 */

#ifndef INC_MASTER_H_
#define INC_MASTER_H_
#include "rs485.h"
#include "global.h"

#define POTENTIOMETER_REGISTER_ADDRESS 0
#define LED_REGISTER_ADDRESS 2
#define LIGHT_REGISTER_ADDRESS 4
#define TEMPERATURE_REGISTER_ADDRESS 6
#define CURRENT_REGISTER_ADDRESS 10
#define VOLTAGE_REGISTER_ADDRESS 14

enum master_state_t{
	IDLE,
	WAITING_TURN_ARROUND_DELAY,
	WAITING_FOR_REPLY,
	PROCESSING_REPLY,
	PROCESSING_ERROR
};
extern modbus_handle_typedef_t master;
float f_master_get_temperature();
float f_master_get_current();
float f_master_get_voltage();
uint16_t f_master_get_light();
uint16_t f_master_get_potention();
void f_rs485_parserFrame(uint8_t *pFrame, uint8_t FrameSize, uint8_t *Address, uint8_t *Function, uint8_t *Data, uint8_t *Datasize, uint16_t *Crc);
void f_master_fsm();
#endif /* INC_MASTER_H_ */
