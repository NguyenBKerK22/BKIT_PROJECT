/*
 * rs485.c
 *
 *  Created on: Nov 24, 2024
 *      Author: ADMIN
 */
#include "rs485.h"
//
//uint8_t led_7seg_colon_index = 0;
//uint8_t slave_address = 0x01;
//
//
//
//User_Mode USER = INIT;
//Modbus_Mode MODBUS = IDLE;
//Modbus_Status Modbus_Send(Modbus_HandleTypeDef* hModbus){
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,SET);
//	if(HAL_UART_Transmit(hModbus->huart,hModbus->Tx_buf,hModbus->Tx_size,HAL_MAX_DELAY) == HAL_OK){
//			return MODBUS_OKE;
//	}
//	return MODBUS_ERROR;
//}
//unsigned char Modbus_CheckData(Modbus_HandleTypeDef* hModbus){
//	uint16_t rx_crc = crc16(hModbus->Rx_buf, hModbus->Rx_size - 2);
//	uint16_t high_crc = hModbus->Rx_buf[hModbus->Rx_size - 1];
//	uint16_t low_crc =  hModbus->Rx_buf[hModbus->Rx_size - 2];
//	uint16_t receive_crc = (high_crc<<8)|(low_crc);
//	return (receive_crc == rx_crc);
//}
//void Modbus_PrepareData(Modbus_HandleTypeDef* hModbus, uint8_t Address, uint8_t Function, uint8_t* Data, uint8_t Size){
//	hModbus->Tx_buf[0] = Address;
//	hModbus->Tx_buf[1] = Function;
//	int i;
//	uint16_t crc = 0;
//	for(i=0; i<Size; i++){
//		hModbus->Tx_buf[i+2] = *(Data + i);
//	}
//	crc = crc16(hModbus->Tx_buf, i+2);
//	hModbus->Tx_buf[i+2] = crc & 0x00FF; // LOW
//	hModbus->Tx_buf[i+3] = crc >> 8; //HIGH
//	hModbus->Tx_size = Size + 4;
//}
//Modbus_Status Modbus_Transmit(Modbus_HandleTypeDef* hModbus, uint32_t Timeout){
//	Modbus_Send(hModbus);
//	//HAL_Delay(5);
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,RESET);
//
//	uint32_t tickStart = HAL_GetTick();
//	while(!Receive_Flag){
//		if(HAL_GetTick()- tickStart >= Timeout){
//				return MODBUS_TIMEOUT;
//		}
//	}
//	Receive_Flag = 0;
//	//HAL_Delay(5);
//	return MODBUS_OKE;
//}
//Modbus_Status Modbus_Receive(Modbus_HandleTypeDef* hModbus){ //Thieu timeout
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,RESET);
//	HAL_UARTEx_ReceiveToIdle_IT(hModbus->huart, hModbus->Rx_buf, MAX_SIZE);
//	return MODBUS_OKE;
//}
//void Modbus_CallBack(Modbus_HandleTypeDef* hModbus, UART_HandleTypeDef* huart, uint16_t Size){
//	if(huart->Instance == hModbus->huart->Instance){
//		Receive_Flag = 1;
//		hModbus->Rx_size = Size;
//		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_4);
//		HAL_UART_Transmit(&huart1,hModbus->Rx_buf,hModbus->Rx_size,100);
//		HAL_UARTEx_ReceiveToIdle_IT(hModbus->huart, hModbus->Rx_buf, MAX_SIZE);
//	}
//
//}
//void Modbus_Init(Modbus_HandleTypeDef* hModbus, UART_HandleTypeDef* huart){
//	led_7seg_set_digit(slave_address>>4, 0, led_7seg_colon_index==0);
//	led_7seg_set_digit(slave_address & 0x0F, 1, led_7seg_colon_index==1);
//	hModbus->huart = huart;
//	hModbus->Address = 0;
//	memset(hModbus->Rx_buf,0,sizeof(hModbus->Rx_buf));
//	hModbus->Rx_size = 256;
//	memset(hModbus->Tx_buf,0,sizeof(hModbus->Tx_buf));
//	hModbus->Tx_size = 0;
//	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,RESET);
//	HAL_UARTEx_ReceiveToIdle_IT(hModbus->huart, hModbus->Rx_buf, MAX_SIZE);
//}
void f_rs485_init_def(uint8_t* tx_buf, uint8_t tx_size, uint8_t* rx_buf){

}
void f_rs485_send_cmd_def(uint8_t* tx_buf, uint8_t tx_size){

}
void f_rs485_fsm_def(){

}
void f_rs485_parserFrame_def(uint8_t *pFrame, uint8_t FrameSize, uint8_t *Address, uint8_t *Function, uint8_t *Data, uint8_t *Datasize, uint16_t *Crc){

}
int f_rs485_received_def(){
	return -1;
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

}

