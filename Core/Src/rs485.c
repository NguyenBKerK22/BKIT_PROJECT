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

UART_HandleTypeDef *_huart;
uint8_t *receive_buffer;
uint8_t *flag_rx;
uint8_t *rx_size;
uint8_t receive_index = 0;

uint8_t transmit_buffer[256] = {0x00};
uint8_t transmit_size = 0;

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

}

/*
 * @brief Send a rs485 cmd
 * @param tx_buffer_without_crc: transmit buffer address, maxsize is 254
 * @param tx_size_without_crc: number of bytes transmit, max is 254
 * @retval: None
 */
void f_rs485_send_cmd(uint8_t *tx_buffer_without_crc, uint8_t tx_size_without_crc)
{

}

void f_rs485_fsm_init()
{

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
			if (is_character_received()) // CHANGE STATE -> RS485_INIT
			{
				_f_init_rs485_init();
				MODBUS485_STATE = RS485_INIT;
			}
			break;
		case RS485_IDLE:
			_f_rs485_idle();

			if (is_transmit()) // CHANGE STATE -> RS485_TRANSMIT
			{
				HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, 1);
				HAL_UART_Transmit(&huart3, transmit_buffer, transmit_size, HAL_MAX_DELAY);
				HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, 0);
				_f_init_rs485_transmit();
				MODBUS485_STATE = RS485_TRANSMIT;
			}
			if (is_character_received()) // CHANGE STATE -> RS485_RECEIVE
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

			if (is_character_received()) // CHANGE STATE -> RS485_RECEIVE
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

}

void _f_rs485_super_init()
{

}

void _f_init_rs485_init()
{

}

void _f_rs485_init()
{

}

void _f_init_rs485_idle()
{

}

void _f_rs485_idle()
{

}

void _f_init_rs485_transmit()
{

}

void _f_rs485_transmit()
{

}

void _f_init_rs485_receive()
{

}

void _f_rs485_receive()
{

}

void _f_init_rs485_waiting_control()
{

}

void _f_rs485_waiting_control()
{

}

void RS485_UART_Callback(UART_HandleTypeDef *huart)
{

}
