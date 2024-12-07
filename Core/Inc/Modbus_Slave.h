/*
 * Modbus_Slave.h
 *
 *  Created on: Dec 5, 2024
 *      Author: ASUS
 */

#ifndef INC_MODBUS_SLAVE_H_
#define INC_MODBUS_SLAVE_H_

#include "main.h"
#include "rs485.h"
#include "string.h"
#include "sensor.h"

#define TEMPERATURE_REGISTER_ADDRESS 0
#define LIGHT_REGISTER_ADDRESS 2
#define CURRENT_REGISTER_ADDRESS 4
#define VOLTAGE_REGISTER_ADDRESS 6
#define POTENTIOMETER_REGISTER_ADDRESS 8

#define SLAVE_ADDRESS 0x01


typedef enum{
	IDLE,
	COMMAND_PARSER,
	READ_MULTIPLE_HOLDING_REGISTER_HANDLER,
	READ_SINGLE_REGISTER_HANDLER
} Slave_behavior;

typedef struct{
	uint8_t s;
} Slave_Subsignal;

typedef struct{
	Slave_Subsignal subsignal;
	Slave_behavior signal;
	uint8_t Register[10];
	uint8_t Address;

} Slave_Device;

extern Slave_Device BKIT01;


void InitSlave(Slave_Device* hDev);
void CommandParser_handler(Slave_Device* hDev);
void Read_multiple_holding_register_handler(Slave_Device* hDev);
void Read_single_register_handler(Slave_Device* hDev);
void slave_behavior(Slave_Device* hDev);


#endif /* INC_MODBUS_SLAVE_H_ */

