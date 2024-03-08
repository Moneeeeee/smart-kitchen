/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
#include "stdint-gcc.h"
#include "retarget.h"
#include "OLED.h"
#include "DHT11.h"
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "MQ2.h"
#include "LED.h"
#include "BEEP.h"
#include "FAN.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Test_Pin GPIO_PIN_13
#define LED_Test_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_1
#define LED_GPIO_Port GPIOA
#define MQ_2_Pin GPIO_PIN_2
#define MQ_2_GPIO_Port GPIOA
#define STEER_Pin GPIO_PIN_3
#define STEER_GPIO_Port GPIOA
#define FAN_Pin GPIO_PIN_4
#define FAN_GPIO_Port GPIOA
#define MOTO_Pin GPIO_PIN_5
#define MOTO_GPIO_Port GPIOA
#define DHT11_Pin GPIO_PIN_0
#define DHT11_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_1
#define BEEP_GPIO_Port GPIOB
#define ESP_TX_Pin GPIO_PIN_10
#define ESP_TX_GPIO_Port GPIOB
#define ESP_RX_Pin GPIO_PIN_11
#define ESP_RX_GPIO_Port GPIOB
#define Debug_TX_Pin GPIO_PIN_9
#define Debug_TX_GPIO_Port GPIOA
#define Debug_RX_Pin GPIO_PIN_10
#define Debug_RX_GPIO_Port GPIOA
#define KEY3_Pin GPIO_PIN_4
#define KEY3_GPIO_Port GPIOB
#define KEY3B5_Pin GPIO_PIN_5
#define KEY3B5_GPIO_Port GPIOB
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
#define KEY1_Pin GPIO_PIN_8
#define KEY1_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_9
#define KEY2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */