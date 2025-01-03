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

/*
 * @brief Send a rs485 cmd
 * @param tx_buffer_without_crc: transmit buffer address, maxsize is 254
 * @param tx_size_without_crc: number of bytes transmit, max is 254
 * @retval: None
 */
void f_rs485_send_cmd(uint8_t* tx_buffer_without_crc, uint8_t tx_size_without_crc)
{

}

/*
 * @brief: RS485 Finite State Machine
 * @retval: None
 */
void f_rs485_fsm()
{

}
