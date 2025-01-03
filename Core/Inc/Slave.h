/*
 * Slave.h
 *
 *  Created on: Jan 3, 2025
 *      Author: ASUS
 */

#ifndef INC_SLAVE_H_
#define INC_SLAVE_H_
#include "main.h"
#include "lcd.h"
#include "rs485.h"
#include <string.h>
#include "sensor.h"

#define SIGNAL_READ_COIL 0x01
#define SIGNAL_READ_DISCRETE_INPUT 0x02
#define SIGNAL_READ_HOLDING_REGISTER 0x03
#define SIGNAL_READ_INPUT_REGISTER 0x04
#define SIGNAL_WRITE_SINGLE_COIL 0x05
#define SIGNAL_WRITE_SINGLE_HOLDING_REGISTER 0x06
#define SIGNAL_WRITE_MULTIPLE_COIL 0x0F
#define SIGNAL_WRITE_MULTIPLE_HOLDING_REGISTER 0x10

#define POTENTIOMETER_REGISTER_ADDRESS 0
#define LED_REGISTER_ADDRESS 2
#define LIGHT_REGISTER_ADDRESS 4
#define TEMPERATURE_REGISTER_ADDRESS 6
#define CURRENT_REGISTER_ADDRESS 10
#define VOLTAGE_REGISTER_ADDRESS 14

#define SLAVE_ADDRESS 0x02

#define COIL_A	1
#define COIL_B	2
#define COIL_C	4
#define COIL_D	8
#define COIL_E	16
#define COIL_F	32

typedef enum{
	STATE_IDLE,
	STATE_WAITTING_FOR_CMD,
	STATE_COMMAND_PARSER,
	STATE_READ_MULTIPLE_HOLDING_REGISTER_HANDLER,
	STATE_READ_HOLDING_REGISTER_HANDLER,
	STATE_WRITE_HOLDING_REGISTER_HANDLER
} slave_state_t;

typedef uint8_t slave_signal_t;

/**
 * @brief Init finite state machine
 */
extern void f_slave_init_def(void);
/**
 * @brief Slave device main finite state machine
 */
extern void f_slave_behavior_def(void);
void f_display_slave_value_def(void);



#endif /* INC_SLAVE_H_ */
