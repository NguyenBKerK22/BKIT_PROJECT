/*
 * global.h
 *
 *  Created on: Dec 14, 2024
 *      Author: ADMIN
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_
#include "stdint.h"
#include "timer.h"
#define BAUDRATE 9600
#define TI_BUTTON_SCAN_TIMER 0
#define TI_BUTTON_SCAN_TIME 20
#define TI_7SEG_SCAN_TIMER 1
#define TI_7SEG_SCAN_TIME 2
#define TI_MODBUS_PERIOD_TIMER 2
#define TI_MODBUS_PERIOD_TIME 1000

#define READ_TEMPERATURE 0x01
#define READ_CURRENT 0x02
#define READ_VOLTAGE 0x03
#define READ_LIGHT 0x04
#define READ_LED 0x05
#define READ_POTENTION 0x06
#define WRITE_TEMPERATURE 0x07
#define WRITE_CURRENT 0x08
#define WRITE_VOLTAGE 0x09
#define WRITE_LIGHT 0x0A
#define WRITE_POTENTION 0x0B
#define BROAD_CAST 0xFF

extern uint8_t slave_address;
extern uint8_t cmd_send;
extern uint8_t send_cmd_flag;
#endif /* INC_GLOBAL_H_ */
