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
#include "string.h"
//#include "retarget.h"
//#include "OLED.h"
//#include "DHT11.h"
//#include "adc.h"
//#include "i2c.h"
//#include "tim.h"
//#include "usart.h"
//#include "gpio.h"
//#include "MQ2.h"
//#include "LED.h"
//#include "BEEP.h"
//#include "FAN.h"
//#include "esp8266.h"
//#include "onenet.h"
//#include "MqttKit.h"
//#include "Steer.h"
//#include "Relay.h"
//#include "Control.h"
//#include "cJSON.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern unsigned char esp8266_buf[128];
extern unsigned short esp8266_cnt , esp8266_cntPre ;

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
#define FAN_Pin GPIO_PIN_4
#define FAN_GPIO_Port GPIOA
#define MOTO_Pin GPIO_PIN_5
#define MOTO_GPIO_Port GPIOA
#define MQ_2_Pin GPIO_PIN_7
#define MQ_2_GPIO_Port GPIOA
#define DHT11_Pin GPIO_PIN_0
#define DHT11_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_1
#define BEEP_GPIO_Port GPIOB
#define STEER_Pin GPIO_PIN_10
#define STEER_GPIO_Port GPIOB
#define Debug_TX_Pin GPIO_PIN_9
#define Debug_TX_GPIO_Port GPIOA
#define Debug_RX_Pin GPIO_PIN_10
#define Debug_RX_GPIO_Port GPIOA
#define KEY1_Pin GPIO_PIN_5
#define KEY1_GPIO_Port GPIOB
#define KEY1_EXTI_IRQn EXTI9_5_IRQn
#define OLED_SCL_Pin GPIO_PIN_6
#define OLED_SCL_GPIO_Port GPIOB
#define OLED_SDA_Pin GPIO_PIN_7
#define OLED_SDA_GPIO_Port GPIOB
#define KEY2_Pin GPIO_PIN_8
#define KEY2_GPIO_Port GPIOB
#define KEY2_EXTI_IRQn EXTI9_5_IRQn
#define KEY3_Pin GPIO_PIN_9
#define KEY3_GPIO_Port GPIOB
#define KEY3_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
