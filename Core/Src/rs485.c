/*
 * rs485.c
 *
 *  Created on: Nov 24, 2024
 *      Author: ADMIN
 */
#include "rs485.h"

void f_rs485_init(uint8_t* tx_buf, uint8_t tx_size, uint8_t* rx_buf){

}

void f_rs485_send_cmd(uint8_t* tx_buf, uint8_t tx_size){

}

void f_rs485_fsm(){

}

void f_rs485_parserFrame(uint8_t *pFrame, uint8_t FrameSize, uint8_t *Address, uint8_t *Function, uint8_t *Data, uint8_t *Datasize, uint16_t *Crc){

}

int f_rs485_received(){
	return -1;
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

}

