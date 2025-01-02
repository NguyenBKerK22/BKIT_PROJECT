/*
 * global.c
 *
 *  Created on: Dec 14, 2024
 *      Author: ADMIN
 */

#include "global.h"

uint8_t slave_address = 0x01;
uint8_t cmd_send = 0x00;
uint8_t flag_send_cmd = 0;
uint8_t flag_slave_not_respond = 0;
uint8_t flag_master_is_idle = 1;
