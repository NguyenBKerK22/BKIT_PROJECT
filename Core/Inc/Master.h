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

#define TI_MASTER_TURN_ARROUND_TIMER 5
#define TI_MASTER_TURN_ARROUND_TIME 50
#define TI_MASTER_WAITING_TIMER 6
#define TI_MASTER_WAITING_TIME 100
enum MASTER_STATE{
	IDLE,
	WAITING_TURN_ARROUND_DELAY,
	WAITING_FOR_REPLY,
	PROCESSING_REPLY,
	PROCESSING_ERROR
};
extern Modbus_HandleTypeDef master;
float f_master_get_temperature_def();
float f_master_get_current_def();
float f_master_get_voltage_def();
uint16_t f_master_get_light_def();
uint16_t f_master_get_potention_def();
void f_rs485_parserFrame_def(uint8_t *pFrame, uint8_t FrameSize, uint8_t *Address, uint8_t *Function, uint8_t *Data, uint8_t *Datasize, uint16_t *Crc);
void f_master_fsm_def();
#endif /* INC_MASTER_H_ */
