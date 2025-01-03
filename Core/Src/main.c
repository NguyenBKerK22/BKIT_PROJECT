/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "global.h"
#include "sensor.h"
#include "user.h"
#include "master.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
modbus_handle_typedef_t master;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void TestADC();
void lcd_run();
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM3){
		timerRun();
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_FSMC_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  led_7seg_init();
  led_7seg_set_colon(1);
  led_7seg_debug_turn_off(6);
  led_7seg_debug_turn_off(7);
  led_7seg_debug_turn_off(8);
  led_7seg_set_digit(0, 0, 0 );
  led_7seg_set_digit(0, 1, 0 );
  led_7seg_set_digit(0, 2, 0 );
  led_7seg_set_digit(0, 3, 0 );
  lcd_init();
  sensor_init();
  lcd_clear(BLACK);
  HAL_TIM_Base_Start_IT(&htim3);
  setTimer(TI_BUTTON_SCAN_TIMER, TI_BUTTON_SCAN_TIME);
  setTimer(TI_7SEG_SCAN_TIMER, TI_7SEG_SCAN_TIME);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(isFlag(TI_BUTTON_SCAN_TIMER)){
		  button_scan();
		  setTimer(TI_BUTTON_SCAN_TIMER, TI_BUTTON_SCAN_TIME);
	  }
	  if(isFlag(TI_7SEG_SCAN_TIMER)){
		  led_7seg_display();
		  setTimer(TI_7SEG_SCAN_TIMER, TI_7SEG_SCAN_TIME);
	  }
	  lcd_run();
	  f_user_interface();
	  f_rs485_fsm();
	  f_master_fsm();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
uint8_t count_adc = 0;
void TestADC() {
	count_adc = (count_adc + 1) % 20;
	if (count_adc == 0) {
		sensor_read();
		lcd_show_string(10, 100, "Voltage:", RED, BLACK, 16, 0);
		lcd_show_float_num(130, 100, sensor_get_voltage(), 4, RED, BLACK, 16);
		lcd_show_string(10, 120, "Current:", RED, BLACK, 16, 0);
		lcd_show_float_num(130, 120, sensor_get_current(), 4, RED, BLACK, 16);
		lcd_show_string(10, 140, "Light:", RED, BLACK, 16, 0);
		lcd_show_int_num(130, 140, sensor_get_light(), 4, RED, BLACK, 16);
		lcd_show_string(10, 160, "Potentiometer:", RED, BLACK, 16, 0);
		lcd_show_int_num(130, 160, sensor_get_potentiometer(), 4, RED, BLACK,
				16);
		lcd_show_string(10, 180, "Temperature:", RED, BLACK, 16, 0);
		lcd_show_float_num(130, 180, sensor_get_temperature(), 4, RED, BLACK,
				16);
	}
}
void lcd_run(){
	lcd_show_string(10, 100, "Temperature:", RED, BLACK, 16, 0);
	lcd_show_float_num(130, 100, f_master_get_temperature(), 4, RED, BLACK, 16);
	lcd_show_string(10, 120, "Current:", RED, BLACK, 16, 0);
	lcd_show_float_num(130, 120, f_master_get_current(), 4, RED, BLACK, 16);
	lcd_show_string(10, 140, "Voltage:", RED, BLACK, 16, 0);
	lcd_show_float_num(130, 140, f_master_get_voltage(), 4, RED, BLACK, 16);
	lcd_show_string(10, 160, "Light:", RED, BLACK, 16, 0);
	lcd_show_int_num(130, 160, f_master_get_light(), 4, RED, BLACK, 16);
	lcd_show_string(10, 180, "Potentiometer:", RED, BLACK, 16, 0);
	lcd_show_int_num(130, 180, f_master_get_potention(), 4, RED, BLACK,16);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
