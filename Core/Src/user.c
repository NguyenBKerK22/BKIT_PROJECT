/*
 * user.c
 *
 *  Created on: Dec 31, 2024
 *      Author: ADMIN
 */
#include "user.h"
enum user_behavior_t user_state = INIT;
uint8_t led_7seg_colon_index = 0;
void f_user_interface(){
	if(flag_slave_not_respond == 1){
		char _buf[22];
		sprintf(_buf,"Slave %d not respond", slave_address);
		lcd_show_string(10, 200, _buf, RED, BLACK, 16, 0);
	}
	switch(user_state){
		case INIT:
			led_7seg_set_digit(slave_address>>4, 0, led_7seg_colon_index==0);
			led_7seg_set_digit(slave_address & 0x0F, 1, led_7seg_colon_index==1);
			user_state = SEND_MANUAL;
			break;
		case SEND_MANUAL:
			if(isPress(0)){
				if(led_7seg_colon_index==0){
					slave_address+= 0x10;
					led_7seg_set_digit(slave_address>>4, 0, led_7seg_colon_index==0);
				}
				else if(led_7seg_colon_index==1){
					slave_address+= 0x01;
					led_7seg_set_digit(slave_address & 0x0F, 1, led_7seg_colon_index==1);
				}
			}
			if(isLongPress(0)){
				if(led_7seg_colon_index==0){
					slave_address+= 0x50;
					led_7seg_set_digit(slave_address>>4, 0, led_7seg_colon_index==0);
				}
				else if(led_7seg_colon_index==1){
					slave_address+= 0x05;
					led_7seg_set_digit(slave_address & 0x0F, 1, led_7seg_colon_index==1);
				}
			}
			if(isPress(4)){
				led_7seg_colon_index++;
				if(led_7seg_colon_index > 3) led_7seg_colon_index = 0;
				led_7seg_set_digit(slave_address>>4, 0, led_7seg_colon_index==0);
				led_7seg_set_digit(slave_address & 0x0F, 1, led_7seg_colon_index==1);
			}
			if(isPress(1)){
				cmd_send = READ_POTENTION;
			}
			if(isPress(2)){
				cmd_send = READ_POTENTION;
			}
			if(isPress(3)){
				cmd_send = READ_LIGHT;
			}
			if(isPress(5)){
				cmd_send = READ_TEMPERATURE;
			}
			if(isPress(6)){
				cmd_send = READ_CURRENT;
			}
			if(isPress(7)){
				cmd_send = READ_VOLTAGE;
			}
			if(isPress(9)){
				cmd_send = READ_LED;
			}
			if(isPress(10)){
				cmd_send = WRITE_LIGHT;
			}
			if(cmd_send != 0x00) flag_send_cmd = 1;
			break;
		case SEND_PERIOD:
			if(flag_master_is_idle == 1){
				cmd_send++;
				flag_send_cmd = 1;
				if(cmd_send >= 0x0B){
					cmd_send = 0x01;
				}
			}
			break;
		default:
			break;
	}
}
