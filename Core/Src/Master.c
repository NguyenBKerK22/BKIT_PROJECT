/*
 * Master.c
 *
 *  Created on: Dec 7, 2024
 *      Author: ADMIN
 */
#include "master.h"
int error_count = 0;
enum master_state_t master_behavior = IDLE;
modbus_handle_typedef_t master;
uint8_t slave_address = 0x01;
uint8_t cmd_send = 0x00;
uint8_t flag_send_cmd = 0;
uint8_t flag_slave_not_respond = 0;
uint8_t flag_master_is_idle = 1;
uint8_t flag_rx = 0;

void _f_master_send_cmd_temperature(){
	master.tx_buf[0] = slave_address;
	master.tx_buf[1] = READ_HOLDING_REGISTER;
	master.tx_buf[2] = 0x00;
	master.tx_buf[3] = TEMPERATURE_REGISTER_ADDRESS;
	master.tx_buf[4] = 0x00;
	master.tx_buf[5] = 0x01;
	master.tx_size = 6;
}

void _f_master_send_cmd_current(){
	master.tx_buf[0] = slave_address;
	master.tx_buf[1] = READ_HOLDING_REGISTER;
	master.tx_buf[2] = 0x00;
	master.tx_buf[3] = CURRENT_REGISTER_ADDRESS;
	master.tx_buf[4] = 0x00;
	master.tx_buf[5] = 0x01;
	master.tx_size = 6;
}

void _f_master_send_cmd_voltage(){
	master.tx_buf[0] = slave_address;
	master.tx_buf[1] = READ_HOLDING_REGISTER;
	master.tx_buf[2] = 0x00;
	master.tx_buf[3] = VOLTAGE_REGISTER_ADDRESS;
	master.tx_buf[4] = 0x00;
	master.tx_buf[5] = 0x01;
	master.tx_size = 6;
}

void _f_master_send_cmd_light(){
	master.tx_buf[0] = slave_address;
	master.tx_buf[1] = READ_HOLDING_REGISTER;
	master.tx_buf[2] = 0x00;
	master.tx_buf[3] = LIGHT_REGISTER_ADDRESS;
	master.tx_buf[4] = 0x00;
	master.tx_buf[5] = 0x01;
	master.tx_size = 6;
}

void _f_master_send_cmd_potentiometer(){
	master.tx_buf[0] = slave_address;
	master.tx_buf[1] = READ_HOLDING_REGISTER;
	master.tx_buf[2] = 0x00;
	master.tx_buf[3] = POTENTIOMETER_REGISTER_ADDRESS;
	master.tx_buf[4] = 0x00;
	master.tx_buf[5] = 0x01;
	master.tx_size = 6;
}

void _f_master_parserFrame(
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
int _f_isFlagReceive(){
	if(flag_rx == 1){
		flag_rx = 0;
		return 1;
	}
	return 0;
}
void f_master_init(){
	master.address = slave_address;
	f_rs485_init(&huart3, master.rx_buf, &flag_rx, &master.rx_size);
	memset(master.tx_buf, 0, sizeof(master.tx_buf));
	master.tx_size = 0;
}

void f_master_fsm(){
	switch(master_behavior){
		case IDLE:
			flag_master_is_idle = 1;
			if(flag_send_cmd){
				flag_send_cmd = 0;
				flag_master_is_idle = 0;
				switch(cmd_send){
					case READ_TEMPERATURE:
						_f_master_send_cmd_temperature();
						break;
					case READ_CURRENT:
						_f_master_send_cmd_current();
						break;
					case READ_VOLTAGE:
						_f_master_send_cmd_voltage();
						break;
					case READ_LIGHT:
						_f_master_send_cmd_light();
						break;
					case READ_POTENTION:
						_f_master_send_cmd_potentiometer();
						break;
					default:
						break;
				}
				f_rs485_send_cmd(master.tx_buf, master.tx_size);
				if(cmd_send == BROAD_CAST){
					setTimer(TI_MASTER_TURN_ARROUND_TIMER, TI_MASTER_TURN_ARROUND_TIME);
					master_behavior = WAITING_TURN_ARROUND_DELAY;
					break;
				}
				setTimer(TI_MASTER_WAITING_TIMER, TI_MASTER_WAITING_TIME);
				master_behavior = WAITING_FOR_REPLY;
			}
			break;
		case WAITING_TURN_ARROUND_DELAY:
			if(isFlag(TI_MASTER_TURN_ARROUND_TIMER)) master_behavior = IDLE;
			break;
		case WAITING_FOR_REPLY:
			if(isFlag(TI_MASTER_TURN_ARROUND_TIMER)) master_behavior = PROCESSING_ERROR;
			else if(_f_isFlagReceive()){
				master_behavior = PROCESSING_REPLY;
			}
			break;
		case PROCESSING_REPLY:
		{
			uint8_t _address;
			uint8_t _function;
			uint8_t _data[256];
			uint8_t _data_size;
			uint16_t _crc_receive;
			_f_master_parserFrame(master.rx_buf, master.rx_size, &_address, &_function, _data,&_data_size, &_crc_receive);
			switch(_function){
				case READ_HOLDING_REGISTER:
				{
					uint8_t _num_bytes = master.rx_buf[2];
					uint16_t _address = (((uint16_t)master.tx_buf[2]<<8)|(master.tx_buf[3]));
					for(int i=0;i<_num_bytes;i++){
						master.holding_register[i + _address]  = master.rx_buf[i+3];
					}
					break;
				}
				case WRITE_MULTIPLE_HOLDING_REGISTER:
					break;
				case WRITE_SINGLE_COIL:
					break;
				default:
					break;
			}
			master_behavior = IDLE;
			break;
		}
		case PROCESSING_ERROR:
			error_count++;
			if(error_count == 5){
				flag_slave_not_respond = 1;
				master_behavior = IDLE;
				break;
			}
			f_rs485_send_cmd(master.tx_buf, master.tx_size);
			if(cmd_send == BROAD_CAST){
				setTimer(TI_MASTER_TURN_ARROUND_TIMER, TI_MASTER_TURN_ARROUND_TIME);
				master_behavior = WAITING_TURN_ARROUND_DELAY;
				break;
			}
			setTimer(TI_MASTER_WAITING_TIMER, TI_MASTER_TURN_ARROUND_TIME);
			master_behavior = WAITING_FOR_REPLY;
			break;
		default:
			break;
	}
}

float f_master_get_temperature(){
	return *((float*)((master.holding_register + TEMPERATURE_REGISTER_ADDRESS)));
}

float f_master_get_current(){
	return *((float*)((master.holding_register + CURRENT_REGISTER_ADDRESS)));;
}

float f_master_get_voltage(){
	return *((float*)((master.holding_register + VOLTAGE_REGISTER_ADDRESS)));
}

uint16_t f_master_get_light(){
	uint16_t _return_val = 0;
	uint8_t _size_of_reg = 2;
	for(int i = 0; i < _size_of_reg; i++){
		_return_val = (_return_val<<8)|(master.holding_register[LIGHT_REGISTER_ADDRESS + i]);
	}
	return _return_val;
}

uint16_t f_master_get_potention(){
	uint16_t _return_val = 0;
	uint8_t _size_of_reg = 2;
	for(int i = 0; i < _size_of_reg; i++){
		_return_val = (_return_val<<8)|(master.holding_register[POTENTIOMETER_REGISTER_ADDRESS + i]);
	}
	return _return_val;
}
