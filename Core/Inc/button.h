/*
 * button.h
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

/* Includes */
#include "stdint.h"
#include "global.h"
/* Variables */
extern uint16_t button_count[16];

/* Functions */
extern void button_init();
extern void button_scan();

extern uint8_t isPress(uint8_t index);
extern uint8_t isLongPress(uint8_t index);
extern void resetButton(uint8_t index);

#endif /* INC_BUTTON_H_ */
