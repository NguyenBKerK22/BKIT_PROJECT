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
#endif /* INC_MASTER_H_ */
