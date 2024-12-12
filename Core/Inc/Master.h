/*
 * Master.h
 *
 *  Created on: Dec 7, 2024
 *      Author: ADMIN
 */

#ifndef INC_MASTER_H_
#define INC_MASTER_H_
#include "rs485.h"

#define IDLE 0x00
#define WAIT_FOR_REPLY 0x01
#define PROCESSING_REPLY 0x02
#define PROCESSING_ERROR 0x03

#define TEMPERATURE_REGISTER_ADDRESS 0
#define LIGHT_REGISTER_ADDRESS 2
#define CURRENT_REGISTER_ADDRESS 4
#define VOLTAGE_REGISTER_ADDRESS 6
#define POTENTIOMETER_REGISTER_ADDRESS 8

extern uint8_t address;
extern uint8_t function;
extern uint8_t data[256];
extern uint8_t size;
extern uint16_t crc_receive;
extern Modbus_HandleTypeDef Master;
extern uint8_t User_behavior;
extern uint8_t Master_behavior;
extern uint8_t Holding_Register[10];
void parserFrame(uint8_t *pFrame, uint8_t FrameSize, uint8_t *Address, uint8_t *Function, uint8_t *Data, uint8_t *Datasize, uint16_t *Crc);
void Master_Run(Modbus_HandleTypeDef* hModbus);
void User_interface();
float Master_get_temperature();
float Master_get_current();
float Master_get_voltage();
uint16_t Master_get_light();
uint16_t Master_get_potention();
#endif /* INC_MASTER_H_ */
