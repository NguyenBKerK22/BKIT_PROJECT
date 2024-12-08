/*
 * sensor.h
 *
 *  Created on: Dec 5, 2024
 *      Author: ADMIN
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_
#include "sensor.h"
#include "main.h"

/* Variables */
extern uint16_t adc_receive[5];
extern ADC_HandleTypeDef hadc1;
/* Functions */
void sensor_init();
void sensor_read();
uint16_t sensor_get_light();
uint16_t sensor_get_potentiometer();

float sensor_get_voltage();
float sensor_get_current();
float sensor_get_temperature();


#endif /* INC_SENSOR_H_ */
