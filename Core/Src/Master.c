/*
 * Master.c
 *
 *  Created on: Dec 7, 2024
 *      Author: ADMIN
 */
#include "master.h"
enum MASTER_STATE _master_behavior = IDLE;
void f_master_fsm_def(){
	switch(_master_behavior){
		case IDLE:
			if(send_cmd_flag){
				switch(cmd_send){
					case READ_TEMPERATURE:
						master.tx_buf[0] = slave_address;
						master.tx_buf[1] = READ_HOLDING_REGISTER;
						master.tx_buf[2] = 0x00;
						master.tx_buf[3] = TEMPERATURE_REGISTER_ADDRESS;
						master.tx_buf[4] = 0x00;
						master.tx_buf[5] = 0x01;
						master.tx_size = 6;
						break;
					case READ_CURRENT:
						master.tx_buf[0] = slave_address;
						master.tx_buf[1] = READ_HOLDING_REGISTER;
						master.tx_buf[2] = 0x00;
						master.tx_buf[3] = CURRENT_REGISTER_ADDRESS;
						master.tx_buf[4] = 0x00;
						master.tx_buf[5] = 0x01;
						master.tx_size = 6;
						break;
					case READ_VOLTAGE:
						master.tx_buf[0] = slave_address;
						master.tx_buf[1] = READ_HOLDING_REGISTER;
						master.tx_buf[2] = 0x00;
						master.tx_buf[3] = VOLTAGE_REGISTER_ADDRESS;
						master.tx_buf[4] = 0x00;
						master.tx_buf[5] = 0x01;
						master.tx_size = 6;
						break;
					case READ_LIGHT:
						master.tx_buf[0] = slave_address;
						master.tx_buf[1] = READ_HOLDING_REGISTER;
						master.tx_buf[2] = 0x00;
						master.tx_buf[3] = LIGHT_REGISTER_ADDRESS;
						master.tx_buf[4] = 0x00;
						master.tx_buf[5] = 0x01;
						master.tx_size = 6;
						break;
					case READ_POTENTION:
						master.tx_buf[0] = slave_address;
						master.tx_buf[1] = READ_HOLDING_REGISTER;
						master.tx_buf[2] = 0x00;
						master.tx_buf[3] = POTENTIOMETER_REGISTER_ADDRESS;
						master.tx_buf[4] = 0x00;
						master.tx_buf[5] = 0x01;
						master.tx_size = 6;
						break;
					default:
						break;
				}
				f_rs485_send_cmd_def(master.tx_buf, master.tx_size);
				if(cmd_send == BROAD_CAST){
					setTimer(TI_MASTER_TURN_ARROUND_TIMER, TI_MASTER_TURN_ARROUND_TIME);
					_master_behavior = WAITING_TURN_ARROUND_DELAY;
					break;
				}
				setTimer(TI_MASTER_WAITING_TIMER, TI_MASTER_TURN_ARROUND_TIME);
				_master_behavior = WAITING_FOR_REPLY;
			}
			break;
		case WAITING_TURN_ARROUND_DELAY:
			if(isFlag(TI_MASTER_TURN_ARROUND_TIMER)) _master_behavior = IDLE;
			break;
		case WAITING_FOR_REPLY:
			if(isFlag(TI_MASTER_TURN_ARROUND_TIME)) _master_behavior = PROCESSING_ERROR;
			else if(f_rs485_received_def()){
				_master_behavior = PROCESSING_REPLY;
			}
			break;
		case PROCESSING_REPLY:
			uint8_t _address_def;
			uint8_t _function_def;
			uint8_t _data_def[256];
			uint8_t _data_size_def;
			uint16_t _crc_receive_def;
			f_rs485_parserFrame_def(master.rx_buf, master.rx_size, &_address_def, &_function_def, _data_def,&_data_size_def, &_crc_receive_def);
			if(_crc_receive_def == crc16(master.rx_buf, master.rx_size - 2)){
					switch(_function_def){
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
				_master_behavior = PROCESSING_ERROR;
			}
			else{
				_master_behavior = PROCESSING_ERROR;
			}
			break;
		case PROCESSING_ERROR:
			f_rs485_send_cmd_def(master.tx_buf, master.tx_size);
			if(cmd_send == BROAD_CAST){
				setTimer(TI_MASTER_TURN_ARROUND_TIMER, TI_MASTER_TURN_ARROUND_TIME);
				_master_behavior = WAITING_TURN_ARROUND_DELAY;
				break;
			}
			setTimer(TI_MASTER_WAITING_TIMER, TI_MASTER_TURN_ARROUND_TIME);
			_master_behavior = WAITING_FOR_REPLY;
			break;
		default:
			break;
	}
}
float f_master_get_temperature_def(){
	return *((float*)((master.holding_register + TEMPERATURE_REGISTER_ADDRESS)));
}
float f_master_get_current_def(){
	return *((float*)((master.holding_register + CURRENT_REGISTER_ADDRESS)));;
}
float f_master_get_voltage_def(){
	return *((float*)((master.holding_register + VOLTAGE_REGISTER_ADDRESS)));
}
uint16_t f_master_get_light_def(){
	uint16_t _return_val_def = 0;
	uint8_t _size_of_reg_def = 2;
	for(int i = 0; i < _size_of_reg_def; i++){
		_return_val_def = (_return_val_def<<8)|(master.holding_register[LIGHT_REGISTER_ADDRESS + i]);
	}
	return _return_val_def;
}
uint16_t f_master_get_potention_def(){
	uint16_t _return_val_def = 0;
	uint8_t _size_of_reg_def = 2;
	for(int i = 0; i < _size_of_reg_def; i++){
		_return_val_def = (_return_val_def<<8)|(master.holding_register[POTENTIOMETER_REGISTER_ADDRESS + i]);
	}
	return _return_val_def;
}
