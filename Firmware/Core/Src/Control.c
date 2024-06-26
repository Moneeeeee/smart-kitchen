//
// Created by Monee on 2024/3/11.
//
#include "Control.h"
#include "adc.h"
#include "DHT11.h"
#include "BEEP.h"
#include "tim.h"
#include "OLED.h"
#include "stdio.h"
volatile uint8_t Mode_Flag = 1;
volatile uint8_t Steer_Flag = 0;
volatile uint8_t Relay_Flag = 0;
volatile uint8_t MQ2_Flag = 0; // 用于指示MQ2传感器值是否超过阈值的标志位
volatile uint8_t MQ4_Flag = 0; // 用于指示MQ2传感器值是否超过阈值的标志位
volatile uint8_t Tem_Flag = 0;
volatile uint8_t FUN_Flag = 0;
volatile uint8_t Water_Flag = 0;
volatile uint8_t Invert_Flag=0;
volatile uint8_t Flash_Flag = 0;



volatile uint8_t tem_threshold = 40;
volatile uint16_t MQ2_threshold = 300;
volatile uint16_t MQ4_threshold = 300;

uint16_t ADC_MQ2;
uint16_t ADC_MQ4;



void Steer_Angle(uint8_t angle)
{
    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,angle + 50); //相当于一个周期内（20ms）有0.5ms高脉冲
    if(angle == 50){
        //当关闭管道，管道位置为1
        Steer_Flag = 1;
    }
    if(angle == 90){
        //当开启管道，管道位置为0
        Steer_Flag = 0;
    }

}


void Steer_Init(void)
{
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
    Steer_Angle(90);
}

//uint16_t ADC_IN_1(void) //ADC采集程序
//{
//    HAL_ADC_Start(&hadc1);//开始ADC采集
//    HAL_ADC_PollForConversion(&hadc1,100);//等待采集结束
//    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))//读取ADC完成标志位
//    {
//        return HAL_ADC_GetValue(&hadc1);//读出ADC数值
//    }
//    return 0;
//}

uint16_t ADC_Read(uint32_t Channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = Channel;                                         /* 通道 */
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;                  /* 采样时间 */
    if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    return (uint16_t)HAL_ADC_GetValue(&hadc1);
}


void MQ_Check()
{
    HAL_ADCEx_Calibration_Start(&hadc1);

    ADC_MQ2 = ADC_Read(ADC_CHANNEL_6);
    ADC_MQ4 = ADC_Read(ADC_CHANNEL_7);


    MQ2_Flag = (ADC_MQ2 > MQ2_threshold) ? 1 : 0;
    MQ4_Flag = (ADC_MQ4 > MQ4_threshold) ? 1 : 0;
}

void Tem_Check()
{
    DHT11_Read_Data(&temperature, &humidity);
    Tem_Flag = (temperature > tem_threshold) ? 1 : 0;
}


void FUN_Cotrol(uint8_t state)
{
    if (state)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
//        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
        FUN_Flag = 1;
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
//        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
        FUN_Flag = 0;
    }

}
void Water_Cotrol(uint8_t state)
{
    if (state)
    {
//        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
        Water_Flag = 1;
    }
    else
    {
//        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
        Water_Flag = 0;
    }

}

void Update_System_Status() {
    MQ_Check();
    Tem_Check();

    if (Mode_Flag == 1) {
        if (Tem_Flag) {
            BEEP_On();
            Flash_Flag = 1;
//            Water_Cotrol(1);
//            Steer_Angle(50);
        }
        //温度指示LED+蜂鸣器
        if(MQ2_Flag){
            BEEP_On();
            Flash_Flag = 2;
            FUN_Cotrol(1);//开启风扇
            Steer_Angle(50);
        }
        //烟雾指示LED+蜂鸣器
        if(MQ4_Flag){
            BEEP_On();
            Flash_Flag = 3;
            Water_Cotrol(1);//消防喷淋
            Steer_Angle(50);
        }
        //燃气指示LED+蜂鸣器
        if(Tem_Flag == 0 && MQ2_Flag == 0){
            BEEP_Off();
            Flash_Flag = 0;
            FUN_Cotrol(0);
            Water_Cotrol(0);//自动模式：风扇、消防喷淋系统关闭
//            Steer_Angle(90);//燃气开关不包括，关闭后需按下按键手动才能开启
        }


    } else {
        if (Tem_Flag) {
            BEEP_On();
            Flash_Flag = 1;
//            Water_Cotrol(1);
//            Steer_Angle(50);
        }
        if(MQ2_Flag){
            BEEP_On();
            Flash_Flag = 2;
            FUN_Cotrol(1);
            Steer_Angle(50);
        }
        if(MQ4_Flag){
            BEEP_On();
            Flash_Flag = 3;
            Water_Cotrol(1);
            Steer_Angle(50);
        }

         if(Tem_Flag == 0 && MQ2_Flag == 0){
           BEEP_Off();
           Flash_Flag = 0;
//           FUN_Cotrol(0);
//           Water_Cotrol(0);
//           Steer_Angle(90);
         }

    }
}


void OLED_Show(void)
{

    char displayString[30]; // 假设足够大以容纳整个字符串
    OLED_ShowNum(10, 6, temperature, 2, 12, 0);
    OLED_ShowNum(40, 6, humidity, 2, 12, 0);
    OLED_ShowNum(70, 6, ADC_MQ2, 3, 12, 0);
    OLED_ShowNum(100,6,ADC_MQ4,3,12, 0);


    OLED_ShowNum(110, 0, tem_threshold, 2, 12, 0);
    OLED_ShowNum(110, 2, MQ2_threshold, 3, 12, 0);
    OLED_ShowNum(110, 4, MQ4_threshold, 3, 12, 0);


    OLED_ShowString(0, 0, Mode_Flag == 1 ? "Mode:  Auto" : "Mode:  Manu", 12, 0);
    OLED_ShowString(0, 2, Steer_Flag == 1 ?"STEER:   ON" : "STEER:  OFF", 12, 0);

    sprintf(displayString, "F/M:%s/%s", FUN_Flag == 1 ? " ON" : "OFF", Water_Flag == 1 ? " ON" : "OFF");
//
//    OLED_ShowString(0, 4, FUN_Flag == 1 ? "FAN: ON" : "FAN:OFF", 12, 0);
//    OLED_ShowString(48, 4, Water_Flag == 1 ? "MOTO: ON" : "MOTO:OFF", 12, 0);
    OLED_ShowString(0, 4, displayString, 12, 0);

}

