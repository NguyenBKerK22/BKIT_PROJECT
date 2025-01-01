/*
 * user.h
 *
 *  Created on: Dec 31, 2024
 *      Author: ADMIN
 */

#ifndef INC_USER_H_
#define INC_USER_H_
#include "global.h"
#include "button.h"
#include "lcd.h"
#include "led_7seg.h"
enum user_behavior_t{
	INIT,
	SEND_MANUAL,
	SEND_PERIOD,
};
extern uint8_t led_7seg_colon_index;
/*
 * @brief: User interface with button
 * @param: None
 * @retval: None
 */
void f_user_interface();
#endif /* INC_USER_H_ */
