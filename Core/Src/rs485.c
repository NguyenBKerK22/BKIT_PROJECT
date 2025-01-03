/*
 * rs485.c
 *
 *  Created on: Nov 24, 2024
 *      Author: ADMIN
 */
#include "rs485.h"
#include "usart.h"
#include "timer.h"
#include "global.h"
#include "crc16.h"

enum modbus485_state_t
{
	RS485_SUPER_INIT,
	RS485_INIT,
	RS485_IDLE,
	RS485_TRANSMIT,
	RS485_RECEIVE,
	RS485_WAITING_CONTROL

};

enum frame_status_t
{
	FRAME_OK,
	FRAME_NOT_OK
};

// FOR DEBUG
#define DEBUG 1
uint8_t msg[100];
void uart_Rs232SendString(uint8_t* str){
	HAL_UART_Transmit(&huart1, (void*)msg, sprintf((void*)msg,"%s",str), HAL_MAX_DELAY);
}
// END FOR

UART_HandleTypeDef *_huart_callback;
uint8_t *_receive_buffer_callback;
uint8_t *_flag_rx_callback;
uint16_t *_rx_size_callback;

uint8_t _receive_byte_buffer = 0;
uint8_t _receive_buffer[256] = {0x00};
uint8_t _receive_index = 0;

uint8_t _transmit_buffer[256] = {0x00};
uint8_t _transmit_size = 0;

enum frame_status_t FRAME_STATUS = FRAME_NOT_OK;
uint8_t _frame_size = 0;
uint8_t _isCRCChecking = 0;

enum modbus485_state_t MODBUS485_STATE = RS485_SUPER_INIT;
uint8_t _character_received_flag = 0;
uint8_t _transmit_flag = 0;

uint8_t _is_frame_status()
{
	if (FRAME_STATUS == FRAME_OK)
	{
		FRAME_STATUS = FRAME_NOT_OK;
		return 1;
	}
	return 0;
}

uint8_t _is_character_received()
{
	if (_character_received_flag == 1)
	{
		_character_received_flag = 0;
		return 1;
	}
	return 0;
}

uint8_t _is_transmit()
{
	if (_transmit_flag == 1)
	{
		_transmit_flag = 0;
		return 1;
	}
	return 0;
}

uint8_t _crc_checking()
{
	if (_receive_index - 2 >= 0)
	{
		uint16_t crc = crc16(_receive_buffer, _receive_index - 2);
		uint8_t crc_high = (crc >> 8) & 0xFF;
		uint8_t crc_low = crc & 0xFF;
		if (crc_low == _receive_buffer[_receive_index - 2] && crc_high == _receive_buffer[_receive_index - 1])
		{
			return 1;
		}
	}

	return 0;
}

/*
 * @brief Initialize values for variable include: huart channel, receive_buffer
 * address, flag_rx and rx_size
 * @param huart: the uart/usart channel
 * @param receive_buffer: a address of the buffer which size is 256
 * @param flag_rx: flag for receive callback fsm
 * @param rx_size: the size of receive command for callback fsm
 * @retval: None
 */
void f_rs485_init(UART_HandleTypeDef *huart, uint8_t *receive_buffer, uint8_t *flag_rx, uint16_t *rx_size)
{
	_huart_callback = huart;
	_receive_buffer_callback = receive_buffer;
	_flag_rx_callback = flag_rx;
	_rx_size_callback = rx_size;
}

/*
 * @brief Send a rs485 cmd
 * @param tx_buffer_without_crc: transmit buffer address, maxsize is 254
 * @param tx_size_without_crc: number of bytes transmit, max is 254
 * @retval: None
 */
void f_rs485_send_cmd(uint8_t *tx_buffer_without_crc, uint8_t tx_size_without_crc)
{
	if (tx_size_without_crc > 254) return;

	for (int i = 0; i < tx_size_without_crc; i++)
	{
		_transmit_buffer[i] = tx_buffer_without_crc[i];
	}
	//	sprintf((void*)transmit_buffer,"%s",cmd_without_crc);
	_transmit_size = tx_size_without_crc;

	uint16_t crc = crc16(_transmit_buffer, _transmit_size);
	_transmit_buffer[_transmit_size] = (uint8_t)(crc & 0x00FF); // CRC low
	_transmit_buffer[_transmit_size + 1] = (uint8_t)((crc >> 8) & 0x00FF); // CRC high

	_transmit_size += 2;
	_transmit_flag = 1;
}

void f_rs485_fsm_init()
{
	// DEBUG
	if (DEBUG) uart_Rs232SendString((uint8_t*)"FSM_INIT\n\r");
	// END DEBUG
	HAL_GPIO_WritePin(EN_RS485_GPIO_Port, EN_RS485_Pin, 0);
	HAL_UART_Receive_IT(_huart_callback, &_receive_byte_buffer, 1);
}

/*
 * @brief: RS485 Finite State Machine, this must be put in while loop
 * @retval: None
 */
void f_rs485_fsm()
{
	switch (MODBUS485_STATE) {
		case RS485_SUPER_INIT:
			_f_rs485_super_init();

			if (1) // CHANGE STATE -> RS485_INIT
			{
				_f_init_rs485_init();
				MODBUS485_STATE = RS485_INIT;
			}
			break;
		case RS485_INIT:
			_f_rs485_init();

			if (isFlag(TI_RS485_T35_TIMER)) // CHANGE STATE -> RS485_IDLE
			{
				_f_init_rs485_idle();
				MODBUS485_STATE = RS485_IDLE;
			}
			if (_is_character_received()) // CHANGE STATE -> RS485_INIT
			{
				_f_init_rs485_init();
				MODBUS485_STATE = RS485_INIT;
			}
			break;
		case RS485_IDLE:
			_f_rs485_idle();

			if (_is_transmit()) // CHANGE STATE -> RS485_TRANSMIT
			{
				HAL_GPIO_WritePin(EN_RS485_GPIO_Port, EN_RS485_Pin, 1);
				HAL_UART_Transmit(_huart_callback, _transmit_buffer, _transmit_size, HAL_MAX_DELAY);
				HAL_GPIO_WritePin(EN_RS485_GPIO_Port, EN_RS485_Pin, 0);
				_f_init_rs485_transmit();
				MODBUS485_STATE = RS485_TRANSMIT;
			}
			if (_is_character_received()) // CHANGE STATE -> RS485_RECEIVE
			{
				_f_init_rs485_receive();
				MODBUS485_STATE = RS485_RECEIVE;
			}
			break;
		case RS485_TRANSMIT:
			_f_rs485_transmit();

			if (isFlag(TI_RS485_T35_TIMER)) // CHANGE STATE -> IDLE
			{
				_f_init_rs485_idle();
				MODBUS485_STATE = RS485_IDLE;
			}
			break;
		case RS485_RECEIVE:
			_f_rs485_receive();

			if (_is_character_received()) // CHANGE STATE -> RS485_RECEIVE
			{
				_f_init_rs485_receive();
				MODBUS485_STATE = RS485_RECEIVE;
			}
			if (isFlag(TI_RS485_T15_TIMER)) // CHANGE STATE -> RS485_WAITING_CONTROL
			{
				_f_init_rs485_waiting_control();
				MODBUS485_STATE = RS485_WAITING_CONTROL;
			}
			break;
		case RS485_WAITING_CONTROL:
			_f_rs485_waiting_control();

			if (isFlag(TI_RS485_T35_TIMER)) // CHANGE STATE -> RS485_IDLE
			{
				if (FRAME_STATUS == FRAME_OK)
				{
					// COPY content in _receive_buffer to _receive_buffer_callback
					for (int i = 0; i < _receive_index; i++)
					{
						_receive_buffer_callback[i] = _receive_buffer[i];
					}

					*_flag_rx_callback = 1;
					*_rx_size_callback = _receive_index;
				}

				_f_init_rs485_idle();
				MODBUS485_STATE = RS485_IDLE;
			}
			break;
		default:
			break;
	}
}

void _f_init_rs485_super_init()
{
	// DEBUG
	if (DEBUG) uart_Rs232SendString((uint8_t*)"S: SUPER_INIT\n\r");
	// END DEBUG
	// Do not thing
}

void _f_rs485_super_init()
{
	// Do not thing
}

void _f_init_rs485_init()
{
	// DEBUG
	if (DEBUG) uart_Rs232SendString((uint8_t*)"S: INIT\n\r");
	// END DEBUG
	setTimer(TI_RS485_T35_TIMER, TI_RS485_T35_TIME);
}

void _f_rs485_init()
{
	// Do not thing
}

void _f_init_rs485_idle()
{
	// DEBUG
	if (DEBUG) uart_Rs232SendString((uint8_t*)"S: IDLE\n\r");
	// END DEBUG
	_transmit_flag = 0;
	_receive_index = 0;
}

void _f_rs485_idle()
{
	// Do not thing
}

void _f_init_rs485_transmit()
{
	// DEBUG
	if (DEBUG) uart_Rs232SendString((uint8_t*)"S: TRANS\n\r");
	// END DEBUG
	setTimer(TI_RS485_T35_TIMER, TI_RS485_T35_TIME);
}

void _f_rs485_transmit()
{
	// Do not thing
}

void _f_init_rs485_receive()
{
	// DEBUG
	if (DEBUG) uart_Rs232SendString((uint8_t*)"S: RECEI\n\r");
	// END DEBUG
	setTimer(TI_RS485_T35_TIMER, TI_RS485_T35_TIME);
	setTimer(TI_RS485_T15_TIMER, TI_RS485_T15_TIME);
}

void _f_rs485_receive()
{
	// Do not thing
}

void _f_init_rs485_waiting_control()
{
	// DEBUG
	if (DEBUG) uart_Rs232SendString((uint8_t*)"S: WAIT\n\r");
	// END DEBUG
	_isCRCChecking = 0;
}

void _f_rs485_waiting_control()
{
	if (_is_character_received())
	{
		FRAME_STATUS = FRAME_NOT_OK;
	}

	if (_isCRCChecking == 0)
	{
		// CHECK SLAVE ADDRESS if needed
		if (_crc_checking())
		{
			FRAME_STATUS = FRAME_OK;
		}
		else
		{
			FRAME_STATUS = FRAME_NOT_OK;
		}
		_frame_size = _receive_index;
		_isCRCChecking = 1;
	}
}

void RS485_UART_Callback(UART_HandleTypeDef *huart)
{
	if (huart == _huart_callback)
	{
		_receive_buffer[_receive_index++] = _receive_byte_buffer;
		_character_received_flag = 1;

		// DEBUG
		HAL_UART_Transmit(&huart1, &_receive_byte_buffer, 1, HAL_MAX_DELAY);
		// END DEBUG

		HAL_UART_Receive_IT(_huart_callback, &_receive_byte_buffer, 1);
	}
}
