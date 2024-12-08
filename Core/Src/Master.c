/*
 * Master.c
 *
 *  Created on: Dec 7, 2024
 *      Author: ADMIN
 */
#include "Master.h"
uint8_t address;
uint8_t function;
uint8_t data[256];
uint8_t data_manual[4] = {0x00,0x01,0x00,0x01};
uint8_t size;
uint16_t crc_receive;
uint8_t User_behavior = INIT;
uint8_t Master_behavior = IDLE;
uint8_t Holding_Register[10]={0};
void parserFrame(
		uint8_t *pFrame,
		uint8_t FrameSize,
		uint8_t *Address,
		uint8_t *Function,
		uint8_t *Data,
		uint8_t *Datasize,
		uint16_t *Crc)
{
	int i;
	*Address = pFrame[0];
	*Function = pFrame[1];
	*Datasize = FrameSize - 4;
	for(i = 0;i < *Datasize; i++){
		Data[i] = *(pFrame + 2 + i);
	}
	*Crc = 0;
	*Crc |= ((uint16_t)(pFrame[FrameSize - 1]) << 8) | ((uint16_t)(pFrame[FrameSize - 2]) & 0x00FF);
}
void User_interface(){
	switch(User_behavior){
		case INIT:
			User_behavior = SEND_MANUAL;
			break;
		case SEND_MANUAL:
			if(isPress(0)){
				User_behavior = SEND_PERIOD;
			}
			if(isPress(1)){ // READ TEMPERATURE
				uint8_t data[4] = {0x00,TEMPERATURE_REGISTER_ADDRESS,0x00,0x01};
				Modbus_PrepareData(&Master, SLAVE_ADDRESS, 0x03, data, 4);
				Modbus_Transmit(&Master, 5000);
				Master_behavior = PROCESSING_REPLY;
//				Modbus_Send(&Master);
//				HAL_Delay(5);
//				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,RESET);
//				Master_behavior = WAIT_FOR_REPLY;
			}
			if(isPress(2)){ // READ HUMID
				uint8_t data[4] = {0x00,HUMIDLITY_REGISTER_ADDRESS,0x00,0x01};
				Modbus_PrepareData(&Master, SLAVE_ADDRESS, 0x03, data, 4);
				Modbus_Transmit(&Master, 5000);
				Master_behavior = PROCESSING_REPLY;
//				Modbus_Send(&Master);
//				HAL_Delay(5);
//				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,RESET);
//				Master_behavior = WAIT_FOR_REPLY;
			}
			if(isPress(3)){ // READ TEMPERATURE AND HUMID
				uint8_t data[4] = {0x00,0x00,0x00,0x02};
				Modbus_PrepareData(&Master, SLAVE_ADDRESS, 0x03, data, 4);
				Modbus_Transmit(&Master, 5000);
				Master_behavior = PROCESSING_REPLY;
//				Modbus_Send(&Master);
//				HAL_Delay(5);
//				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,RESET);
//				Master_behavior = WAIT_FOR_REPLY;
			}
			if(isPress(4)){// WRITE SINGLE COIL

			}
			if(isPress(5)){// WRITE SINGLE HOLDING REGISTER

			}
			break;
		case SEND_PERIOD:
			if(isPress(0)){
				User_behavior = SEND_MANUAL;
			}
			//if(timer4_flag){
				//timer4_flag = 0;
				Modbus_PrepareData(&Master, SLAVE_ADDRESS, 0x03, data_manual, 4);
				Modbus_Transmit(&Master, 5000);
				Master_behavior = PROCESSING_REPLY;
//				Modbus_Send(&Master);
//				HAL_Delay(5);
//				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,RESET);
//				Master_behavior = WAIT_FOR_REPLY;
			//}
			break;
		default:
			break;
	}
}
void Master_Run(Modbus_HandleTypeDef* hModbus){
	switch(Master_behavior){
		case IDLE:
			break;
//		case WAIT_FOR_REPLY:
//				uint32_t tickStart = HAL_GetTick();
//				while(Receive_Flag != 1){
//					if(HAL_GetTick() - tickStart >= 5000){
//						Master_behavior = PROCESSING_ERROR;
//						break;
//					}
//				}
//				if(Receive_Flag == 1){
//					Master_behavior = PROCESSING_REPLY;
//				}
//			break;
		case PROCESSING_REPLY:
				if(hModbus->Rx_size < 4){
					Master_behavior = PROCESSING_ERROR;
					break;
				}
				parserFrame(hModbus->Rx_buf,hModbus->Rx_size ,&address, &function, data, &size, &crc_receive);
				if(crc_receive == crc16(hModbus->Rx_buf, size + 2)){
					switch(function){
						case READ_HOLDING_REGISTER:
							uint8_t num_bytes = hModbus->Rx_buf[2];
							uint16_t address = (((uint16_t)hModbus->Tx_buf[2]<<8)|(hModbus->Tx_buf[3]));
							for(int i=0;i<num_bytes;i++){
								Holding_Register[i + address * SizeOfReg]  = hModbus->Rx_buf[i+3];
							}
//							HAL_UART_Transmit(&huart1, Holding_Register, 10, HAL_MAX_DELAY);
							break;
						case WRITE_MULTIPLE_HOLDING_REGISTER:
							break;
						case WRITE_SINGLE_COIL:
							break;
						default:
							break;
					}
					Master_behavior = IDLE;
				}
				else{
					Master_behavior = PROCESSING_ERROR;
				}
			break;
		case PROCESSING_ERROR:
//				Modbus_Send(hModbus);
//				HAL_Delay(5);
//				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,RESET);
//				Master_behavior = WAIT_FOR_REPLY;
				Modbus_Transmit(&Master, 5000);
				Master_behavior = PROCESSING_REPLY;
			break;
		default:
			break;
	}
}
float Master_get_temperature(){
	uint16_t res = 0;
	for(int i = 0; i < SizeOfReg; i++){
		res = (res<<8)|(Holding_Register[TEMPERATURE_REGISTER_ADDRESS*SizeOfReg + i]);
	}
	return (float)res/10;
}
float Master_get_humid(){
	uint16_t res = 0;
	for(int i = 0; i < SizeOfReg; i++){
		res = (res<<8)|(Holding_Register[HUMIDLITY_REGISTER_ADDRESS*SizeOfReg + i]);
	}
	return (float)res/10;
}
