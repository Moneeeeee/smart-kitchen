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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    const char *topics[] = {"/mysmartkitchen/sub"};

    unsigned short timeCount = 0;	//

    unsigned char *dataPtr = NULL;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

//    include_directories(
//            Core/Inc
//    Drivers/STM32F1xx_HAL_Driver/Inc
//    Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
//    Drivers/CMSIS/Device/ST/STM32F1xx/Include
//    Drivers/CMSIS/Include
//    NET/
//    )
//
//    add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB)
//
//    file(GLOB_RECURSE SOURCES
//    "Core/*.*"
//    "Drivers/*.*"
//    "NET/*.*"
//    )
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  RetargetInit(&huart1);
  BEEP_Init();
  OLED_Init();                           //OLED初始
  OLED_Clear();                         //清屏
  Steer_Init();



  printf("The USART1 is OK!\r\n");
  HAL_UART_Receive_IT(&huart2, &aRxBuffer, 1); // 启动中断接收
  ESP01S_Init();  //8266初始
  while(OneNet_DevLink())  //接入onenet
  ESP01S_Clear();    //*/

  OneNet_Subscribe(topics, 1);

    OLED_ShowString(0,0,"Mode:   Auto",12, 0);
    OLED_ShowString(0,2,"STEER:   OFF",12,0);
    OLED_ShowString(0,4,"FAN/MOTO:OFF",12,0);

    OLED_ShowString(10,7,"Tem",12, 0);
    OLED_ShowString(50,7,"Hum",12, 0);
    OLED_ShowString(90,7,"MQ2",12, 0);

  printf("Init is OK!\r\n");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {


      HAL_ADCEx_Calibration_Start(&hadc1);//ADC采样校准
      ADC_MQ2 = ADC_IN_1();

      DHT11_Read_Data(&temperature, &humidity);

      OLED_Show();

      if(Flash_Flag == 1)
      {
          HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
      }

      HAL_Delay(100);
      /*****继电�??????????-风扇、电�??????????
          MOTO_Cotrol(1);
          FUN_Cotrol(1);
       ***/




    /************舵机

           Steer_Angle(50);
          HAL_Delay(1000);

          Steer_Angle(100);
          HAL_Delay(1000);

          Steer_Angle(150);
          HAL_Delay(1000);

     ******************/

      /*********蜂鸣�???????????

        Buzzer_Beep(uint32_t onTimeMs, uint32_t offTimeMs, uint8_t repetitions);
        BEEP_Init();
        BEEP_On();
        BEEP_Off();

       ***********/

      /*****MQ-2烟雾

      HAL_ADCEx_Calibration_Start(&hadc1);//ADC采样校准
      ADC_MQ2 = ADC_IN_1();
      printf("%d\r\n",ADC_MQ2);

       **********/

      /************DHT11温湿度读�????????????

      DHT11_Read_Data(&temperature , &humidity);
      //将数据存放到aTXbuf这个数组当中去�?? 不了�????????????"sprintf"用法的可以去查一�????????????...
      sprintf((char*)aTXbuf,"温度�????????????%d℃，湿度: %d %%\r\n" ,temperature ,humidity);
      //将数据过串口发到主机上的串口助手
      HAL_UART_Transmit(&huart1, aTXbuf, strlen((const char*)aTXbuf), 200);
      HAL_Delay(500);

       **********/

      /**************上传MQTT

       if(++timeCount >= 50)									//发�?�间�???????????????5s
      {

          printf("OneNet_Publish\r\n");

          OneNet_Publish("pcTopic", "MQTT Publish Test");

          timeCount = 0;
          ESP01S_Clear();
      }
      dataPtr = ESP01S_GetIPD(0);


      if(dataPtr != NULL)
          OneNet_RevPro(dataPtr);

       ******************/




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    printf("111\r\n");
    if(ESP01S_cnt >= sizeof(ESP01S_buf))  //溢出判断
    {
        ESP01S_cnt = 0;
        memset(ESP01S_buf,0x00,sizeof(ESP01S_buf));
        HAL_UART_Transmit(&huart1, (uint8_t *)"接收缓存溢出", 10,0xFFFF);
    }
    else
    {
        ESP01S_buf[ESP01S_cnt++] = aRxBuffer;   //接收数据转存
//		  if(aRxBuffer=='1')  HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
//        if(aRxBuffer=='0')  HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
    }

    HAL_UART_Receive_IT(&huart2, &aRxBuffer, 1);   //再开启接收中�???????????????
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
