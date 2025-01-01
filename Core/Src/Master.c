/*
 * Master.c
 *
 *  Created on: Dec 7, 2024
 *      Author: ADMIN
 */
#include "master.h"
enum master_state_t master_behavior = IDLE;
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

void f_master_fsm(){
	switch(master_behavior){
		case IDLE:
			if(flag_send_cmd){
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
				setTimer(TI_MASTER_WAITING_TIMER, TI_MASTER_TURN_ARROUND_TIME);
				master_behavior = WAITING_FOR_REPLY;
			}
			break;
		case WAITING_TURN_ARROUND_DELAY:
			if(isFlag(TI_MASTER_TURN_ARROUND_TIMER)) master_behavior = IDLE;
			break;
		case WAITING_FOR_REPLY:
			if(isFlag(TI_MASTER_TURN_ARROUND_TIME)) master_behavior = PROCESSING_ERROR;
			else if(f_rs485_received()){
				master_behavior = PROCESSING_REPLY;
			}
			break;
		case PROCESSING_REPLY:
			uint8_t _address;
			uint8_t _function;
			uint8_t _data[256];
			uint8_t _data_size;
			uint16_t _crc_receive;
			f_rs485_parserFrame(master.rx_buf, master.rx_size, &_address, &_function, _data,&_data_size, &_crc_receive);
			if(_crc_receive == crc16(master.rx_buf, master.rx_size - 2)){
					switch(_function){
					case READ_HOLDING_REGISTER:
						uint8_t _num_bytes = master.rx_buf[2];
						uint16_t _address = (((uint16_t)master.tx_buf[2]<<8)|(master.tx_buf[3]));
						for(int i=0;i<_num_bytes;i++){
							master.holding_register[i + _address]  = master.rx_buf[i+3];
						}
						break;
					case WRITE_MULTIPLE_HOLDING_REGISTER:
						break;
					case WRITE_SINGLE_COIL:
						break;
					default:
						break;
					}
				master_behavior = PROCESSING_ERROR;
			}
			else{
				master_behavior = PROCESSING_ERROR;
			}
			break;
		case PROCESSING_ERROR:
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
