/*
 * rs485.c
 *
 *  Created on: Nov 24, 2024
 *      Author: ADMIN
 */
#include "rs485.h"

/*
 * @brief Initialize values for variable include: huart channel, receive_buffer
 * address, flag_rx and rx_size
 * @param huart: the uart/usart channel
 * @param receive_buffer: a address of the buffer which size is 256
 * @param flag_rx: flag for receive callback fsm
 * @param rx_size: the size of receive command for callback fsm
 * @retval: None
 */
void f_rs485_init(UART_HandleTypeDef *huart, uint8_t* receive_buffer, uint8_t* flag_rx, uint8_t* rx_size)
{

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

