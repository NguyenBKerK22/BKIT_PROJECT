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

void f_rs485_init(UART_HandleTypeDef *huart, uint8_t* receive_buffer, uint8_t* flag_rx, uint8_t* rx_size);

/*
 * @brief: Send command without CRC
 * @param: tx_buf - Transmit buffer
 * @param: tx_size - Number of bytes transmit
 * @retval: None
 */
void f_rs485_send_cmd(uint8_t* tx_buf, uint8_t tx_size);

/*
 * @brief: RS485 Finite State Machine
 * @param: None
 * @retval: None
 */
void f_rs485_fsm();

/*
 * @brief:
 * @param: pFrame -
 * @param: FrameSize -
 * @param: Address -
 * @param: Function -
 * @param: Data -
 * @param: Datasize -
 * @param: Crc -
 * @retval: None
 */
void f_rs485_parserFrame(uint8_t *pFrame, uint8_t FrameSize, uint8_t *Address, uint8_t *Function, uint8_t *Data, uint8_t *Datasize, uint16_t *Crc);

/*
 * @brief: Check received flag
 * @param: None
 * @retval: Received flag value
 */
int f_rs485_received();
typedef struct{
	UART_HandleTypeDef* huart;
	uint8_t address;
	uint8_t tx_buf[256];
	uint16_t tx_size;
	uint8_t rx_buf[256];
	uint16_t rx_size;
	uint8_t holding_register[18];
}modbus_handle_typedef_t;
#endif /* INC_RS485_H_ */
