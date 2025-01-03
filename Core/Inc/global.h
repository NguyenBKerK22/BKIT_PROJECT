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
#define TI_RS_485_TIMER 1000
#define TI_RS_485_TIM 1000
#define TI_MASTER_TURN_ARROUND_TIMER 5
#define TI_MASTER_TURN_ARROUND_TIME 50
#define TI_MASTER_WAITING_TIMER 6
#define TI_MASTER_WAITING_TIME 100

#define TI_RS485_T15_TIMER 7
#define TI_RS485_T15_TIME 1 // 1000ms / (RS485_BAUDRATE / 10) * 1.5 = 1.5625 ms

#define TI_RS485_T35_TIMER 8
#define TI_RS485_T35_TIME 4 // 1000ms / (RS485_BAUDRATE / 10) * 3.5 = 3.645 ms

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
extern uint8_t flag_send_cmd;
extern uint8_t flag_slave_not_respond;
extern uint8_t flag_master_is_idle;
#endif /* INC_GLOBAL_H_ */
