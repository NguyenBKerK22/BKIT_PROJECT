/*
 * rs485.h
 *
 *  Created on: Nov 24, 2024
 *      Author: ADMIN
 */

#ifndef INC_RS485_H_
#define INC_RS485_H_

#include "main.h"
#include "crc16.h"
#include "global.h"
#include "usart.h"

void f_rs485_init(UART_HandleTypeDef *huart, uint8_t *receive_buffer, uint8_t *flag_rx, uint8_t *rx_size);

void f_rs485_send_cmd(uint8_t *tx_buffer_without_crc, uint8_t tx_size_without_crc);

void f_rs485_fsm_init();

void f_rs485_fsm();

void RS485_UART_Callback(UART_HandleTypeDef *huart);

void _f_init_rs485_super_init();
void _f_rs485_super_init();
void _f_init_rs485_init();
void _f_rs485_init();
void _f_init_rs485_idle();
void _f_rs485_idle();
void _f_init_rs485_transmit();
void _f_rs485_transmit();
void _f_init_rs485_receive();
void _f_rs485_receive();
void _f_init_rs485_waiting_control();
void _f_rs485_waiting_control();


#endif /* INC_RS485_H_ */
