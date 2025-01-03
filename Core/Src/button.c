/*
 * button.c
 */

/* Includes */
#include "button.h"

#include "spi.h"
#include "gpio.h"
#include "main.h"

#define TIME_OUT_FOR_LONG_PRESS 500 // ms

/* Chong rung */
uint16_t isPressed[16] = {0};
uint16_t isLongPressed[16] = {0};
uint16_t timeOutForPress[16] = {500};

uint16_t button_count3[16] = {0};
uint16_t button_count2[16] = {0};
uint16_t button_count1[16] = {0};

/* Variables */
uint16_t button_count[16] = {0};
static uint16_t button_spi_buffer = 0x0000;

/* Functions */
/**
 * @brief  	Init matrix button
 * @param  	None
 * @retval 	None
 */
void button_init() {
	HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 1);
}

/**
 * @brief  	Scan matrix button
 * @param  	None
 * @note  	Call every 50ms
 * @retval 	None
 */
void button_scan() {
	// Shift button input
	for (int i = 0; i < 16; i++)
	{
		button_count2[i] = button_count1[i];
		button_count1[i] = button_count[i];
	}

	// Get new key input to button_count
	HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 0);
	HAL_GPIO_WritePin(BTN_LOAD_GPIO_Port, BTN_LOAD_Pin, 1);
	HAL_SPI_Receive(&hspi1, (void*) &button_spi_buffer, 2, 10);

	int button_index = 0;
	uint16_t mask = 0x8000;
	for (int i = 0; i < 16; i++) {
		if (i >= 0 && i <= 3) {
			button_index = i + 4;
		} else if (i >= 4 && i <= 7) {
			button_index = 7 - i;
		} else if (i >= 8 && i <= 11) {
			button_index = i + 4;
		} else {
			button_index = 23 - i;
		}
		if (button_spi_buffer & mask)
			button_count[button_index] = 0;
		else
			button_count[button_index] = 1;
		mask = mask >> 1;
	}

	for (int i = 0; i < 16; i++)
	{
		if ((button_count[i] == button_count1[i]) && (button_count1[i] == button_count2[i]))
		{
			if (button_count2[i] != button_count3[i])
			{
				button_count3[i] = button_count2[i];

				if (button_count3[i] == 1) // PRESSED
				{
					timeOutForPress[i] = TIME_OUT_FOR_LONG_PRESS / TI_BUTTON_SCAN_TIME;
					isPressed[i] = 1;
				}
			}
			else
			{
				timeOutForPress[i]--;
				if (timeOutForPress[i] <= 0)
				{
					timeOutForPress[i] = TIME_OUT_FOR_LONG_PRESS / TI_BUTTON_SCAN_TIME;
					if (button_count3[i] == 1) // PRESSED
					{
						isLongPressed[i] = 1;
					}
				}
			}
		}
	}
}

uint8_t isPress(uint8_t index)
{
	if (index < 0 || index > 15) return 0;
	if (isPressed[index] == 1)
	{
		isPressed[index] = 0;
		return 1;
	}
	return 0;
}

uint8_t isLongPress(uint8_t index)
{
	if (index < 0 || index > 15) return 0;
	if (isLongPressed[index] == 1)
	{
		isLongPressed[index] = 0;
		return 1;
	}
	return 0;
}

void resetButton(uint8_t index)
{
	isPressed[index] = 0;
	isLongPressed[index] = 0;
}
