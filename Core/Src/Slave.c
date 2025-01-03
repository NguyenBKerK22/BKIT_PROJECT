/*
 * Slave.c
 *
 *  Created on: Jan 3, 2025
 *      Author: ASUS
 */
#include "Slave.h"

static uint8_t _register_def[20];
static uint8_t _address_def;
static slave_signal_t _slave_signal_def;
static slave_state_t _slave_state_def;
uint8_t tx_buf[256]; // for dummy
uint8_t rx_buf[256];
uint8_t flag_rx;
uint8_t rx_size;
uint8_t tx_size; // for dummy


static void _f_splitfloat_def(float _input, uint8_t *data)
{
	data[0] = *(((uint8_t*)&_input) + 0);
	data[1] = *(((uint8_t*)&_input) + 1);
	data[2] = *(((uint8_t*)&_input) + 2);
	data[3] = *(((uint8_t*)&_input) + 3);
}

static void _f_splituint16_def(uint16_t input, uint8_t *higherByte, uint8_t *lowerByte)
{
    *higherByte = (input >> 8) & 0xFF; // Lấy 8 bit cao
    *lowerByte = input & 0xFF;        // Lấy 8 bit thấp
}

static void _f_slave_read_multiple_holding_register_handler_def(void)
{
	uint16_t startReg = (uint16_t)(rx_buf[2]) << 8 | (uint16_t)(rx_buf[3]);
	uint16_t numberOfReg = (uint16_t)(rx_buf[4]) << 8 | (uint16_t)(rx_buf[5]);
	tx_buf[0] = _address_def;
	tx_buf[1] = rx_buf[1];
	tx_buf[2] = numberOfReg * 2;
	int i;
	for(i = 0; i < numberOfReg * 2; i++){
		tx_buf[i + 3] = _register_def[startReg];
	}
	tx_size = i + 3;
	rs485_send_cmd(tx_buf, tx_size);

	//	Modbus_Transmit_Slave(&slave, hDev->Address, slave.Rx_buf[1], &hDev->Register[startReg], numberOfReg * 2, 100);

	return;

}

static void _f_slave_read_holding_register_handler_def(void)
{
	return;
}

static void _f_slave_write_holding_register_handler_def(void)
{
	rs485_send_cmd(rx_buf, rx_size);
	_register_def[LED_REGISTER_ADDRESS] = rx_buf[4];
	_register_def[LED_REGISTER_ADDRESS + 1] = rx_buf[5];
	uint8_t CoilState = _register_def[LED_REGISTER_ADDRESS + 1];
}

static void _f_slave_commandparser_handler_def(void)
{
	switch(1){
	case SIGNAL_READ_COIL:
	{
		break;
	}
	case SIGNAL_READ_DISCRETE_INPUT:
	{
		break;
	}
	case SIGNAL_READ_HOLDING_REGISTER:
	{
		_slave_state_def = STATE_READ_HOLDING_REGISTER_HANDLER;
		break;
	}
	case SIGNAL_READ_INPUT_REGISTER:
	{
		break;
	}
	case SIGNAL_WRITE_MULTIPLE_COIL:
	{
		break;
	}
	case SIGNAL_WRITE_MULTIPLE_HOLDING_REGISTER:
	{
		_slave_state_def = STATE_READ_MULTIPLE_HOLDING_REGISTER_HANDLER;
		break;
	}
	case SIGNAL_WRITE_SINGLE_COIL:
	{
		break;
	}
	case SIGNAL_WRITE_SINGLE_HOLDING_REGISTER:
	{
		_slave_state_def = STATE_WRITE_HOLDING_REGISTER_HANDLER;
		break;
	}
	default:
		break;
	}
}

static void _f_read_data_def(void){
	float tempCur = sensor_get_current();
	uint16_t tempLt = sensor_get_light();
	uint16_t tempPt =sensor_get_potentiometer();
	float tempT = sensor_get_temperature();
	float tempVol = sensor_get_voltage();

	// Current
	_f_splitfloat_def(tempCur, &_register_def[CURRENT_REGISTER_ADDRESS]);
	// Temperature
	_f_splitfloat_def(tempT, &_register_def[TEMPERATURE_REGISTER_ADDRESS]);
	// Voltage
	_f_splitfloat_def(tempVol, &_register_def[VOLTAGE_REGISTER_ADDRESS]);
	// Light
	_f_splituint16_def(tempLt, &_register_def[LIGHT_REGISTER_ADDRESS], &_register_def[LIGHT_REGISTER_ADDRESS + 1]);
	// Potentiometer
	_f_splituint16_def(tempPt, &_register_def[POTENTIOMETER_REGISTER_ADDRESS], &_register_def[POTENTIOMETER_REGISTER_ADDRESS + 1]);
}



void f_slave_init_def(void)
{
	_slave_signal_def = 0;
	_slave_state_def = STATE_IDLE;
	_address_def = SLAVE_ADDRESS;
	memset(_register_def, 0, sizeof(_register_def));
}

void f_slave_behavior_def(void)
{
	switch(_slave_state_def)
	{
	case STATE_IDLE:
	{
		break;
	}
	case STATE_WAITTING_FOR_CMD:
	{
		_f_read_data_def();
		if(1){
			_slave_state_def = STATE_COMMAND_PARSER;
		}
		break;
	}
	case STATE_COMMAND_PARSER:
	{
		_f_slave_commandparser_handler_def();
		break;
	}
	case STATE_READ_MULTIPLE_HOLDING_REGISTER_HANDLER:
	{
		_f_slave_read_multiple_holding_register_handler_def();
		if(1){
			_slave_state_def = STATE_IDLE;
		}
		break;
	}
	case STATE_READ_HOLDING_REGISTER_HANDLER:
	{
		_f_slave_read_holding_register_handler_def();
		if(1){
			_slave_state_def = STATE_IDLE;
		}
		break;
	}
	case STATE_WRITE_HOLDING_REGISTER_HANDLER:
	{
		_f_slave_write_holding_register_handler_def();
		if(1){
			_slave_state_def = STATE_IDLE;
		}
		break;
	}
	default:
		break;
	}
}



