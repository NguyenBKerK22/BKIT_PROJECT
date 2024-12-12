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
#include "string.h"

extern int data_come;
extern UART_HandleTypeDef huart3;

#define READ_COIL 0x01
#define READ_DISCRETE_INPUT 0x02
#define READ_HOLDING_REGISTER 0x03
#define READ_INPUT_REGISTER 0x04
#define WRITE_SINGLE_COIL 0x05
#define WRITE_SINGLE_HOLDING_REGISTER 0x06
#define WRITE_MULTIPLE_COIL 0x0F
#define WRITE_MULTIPLE_HOLDING_REGISTER 0x10

#define MAX_SIZE 256

extern unsigned char Receive_Flag;
typedef enum{
	MODBUS_TIMEOUT = 0,
	MODBUS_ERROR,
	MODBUS_OKE
}Modbus_Status;
typedef struct{
	UART_HandleTypeDef* huart;
	uint8_t Address;
	uint8_t Tx_buf[256];
	uint16_t Tx_size;
	uint8_t Rx_buf[256];
	uint16_t Rx_size;
	unsigned char RxFlag;

}Modbus_HandleTypeDef;
extern Modbus_HandleTypeDef Master;
extern UART_HandleTypeDef huart1;
void Modbus_CallBack(Modbus_HandleTypeDef* hModbus, UART_HandleTypeDef* huart, uint16_t Size);
void Modbus_Init(Modbus_HandleTypeDef* hModbus, UART_HandleTypeDef* huart);
unsigned char Modbus_CheckData(Modbus_HandleTypeDef* hModbus);
Modbus_Status Modbus_Transmit(Modbus_HandleTypeDef* hModbus, uint8_t Address, uint8_t Function, uint8_t* Data, uint8_t Size, uint32_t Timeout);
Modbus_Status Modbus_Receive(Modbus_HandleTypeDef* hModbus);
#endif /* INC_RS485_H_ */
