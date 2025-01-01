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
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
#define READ_COIL 0x01
#define READ_DISCRETE_INPUT 0x02
#define READ_HOLDING_REGISTER 0x03
#define READ_INPUT_REGISTER 0x04
#define WRITE_SINGLE_COIL 0x05
#define WRITE_SINGLE_HOLDING_REGISTER 0x06
#define WRITE_MULTIPLE_COIL 0x0F
#define WRITE_MULTIPLE_HOLDING_REGISTER 0x10
/*
 * @brief: Initialize value for variable
 * @param: tx_buf - Transmit buffer
 * @param: tx_size - Number of bytes transmit
 * @param: rx_buf - Receive buffer
 * @retval: None
 */
void f_rs485_init(uint8_t* tx_buf, uint8_t tx_size, uint8_t* rx_buf);

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
