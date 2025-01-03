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
#endif /* INC_GLOBAL_H_ */
