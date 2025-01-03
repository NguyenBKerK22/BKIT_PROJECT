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
#include "string.h"

#define POTENTIOMETER_REGISTER_ADDRESS 0
#define LED_REGISTER_ADDRESS 2
#define LIGHT_REGISTER_ADDRESS 4
#define TEMPERATURE_REGISTER_ADDRESS 6
#define CURRENT_REGISTER_ADDRESS 10
#define VOLTAGE_REGISTER_ADDRESS 14

#define READ_COIL 0x01
#define READ_DISCRETE_INPUT 0x02
#define READ_HOLDING_REGISTER 0x03
#define READ_INPUT_REGISTER 0x04
#define WRITE_SINGLE_COIL 0x05
#define WRITE_SINGLE_HOLDING_REGISTER 0x06
#define WRITE_MULTIPLE_COIL 0x0F
#define WRITE_MULTIPLE_HOLDING_REGISTER 0x10

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
enum master_state_t{
	IDLE,
	WAITING_TURN_ARROUND_DELAY,
	WAITING_FOR_REPLY,
	PROCESSING_REPLY,
	PROCESSING_ERROR
};
typedef struct{
	UART_HandleTypeDef* huart;
	uint8_t address;
	uint8_t tx_buf[256];
	uint16_t tx_size;
	uint8_t rx_buf[256];
	uint16_t rx_size;
	uint8_t holding_register[18];
}modbus_handle_typedef_t;

extern modbus_handle_typedef_t master;
extern uint8_t slave_address;
extern uint8_t cmd_send;
extern uint8_t flag_send_cmd;
extern uint8_t flag_slave_not_respond;
extern uint8_t flag_master_is_idle;
extern uint8_t flag_rx;
/*
 * @brief: Get temperature value in holding register
 * @param: None
 * @retval: Temperature value
 */
float f_master_get_temperature();
/*
 * @brief: Get current value in holding register
 * @param: None
 * @retval: Current value
 */
float f_master_get_current();
/*
 * @brief: Get voltage value in holding register
 * @param: None
 * @retval: Voltage value
 */
float f_master_get_voltage();
/*
 * @brief: Get light value in holding register
 * @param: None
 * @retval: Light value
 */
uint16_t f_master_get_light();
/*
 * @brief: Get potention value in holding register
 * @param: None
 * @retval: Potention value
 */
uint16_t f_master_get_potention();
/*
 * @brief: Master Finite State Machine
 * @param: None
 * @retval: None
 */
void f_master_fsm();
/*
 * @brief: Master Initialize
 * @param: None
 * @retval: None
 */
void f_master_init();
#endif /* INC_MASTER_H_ */
