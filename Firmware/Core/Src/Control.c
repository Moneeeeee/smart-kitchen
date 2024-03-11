//
// Created by Monee on 2024/3/11.
//
#include "Control.h"

volatile uint8_t Mode_Flag = 1;
volatile uint8_t Steer_Flag = 0;
volatile uint8_t Relay_Flag = 0;
volatile uint8_t MQ2_Flag = 0; // 用于指示MQ2传感器值是否超过阈值的标志位
volatile uint8_t Tem_Flag = 0;
volatile uint8_t Hum_Flag = 0;

void MQ2_Check()
{
    HAL_ADCEx_Calibration_Start(&hadc1);//ADC采样校准
    ADC_MQ2 = ADC_IN_1();

    if (ADC_MQ2 > 1233) {
        MQ2_Flag = 1; // 如果ADC_MQ2的值超过阈值，设置MQ2_Flag为1
    } else {
        MQ2_Flag = 0; // 如果ADC_MQ2的值没有超过阈值，保持MQ2_Flag为0或者将其设置为0
    }
}

void Tem_Check()
{
    if (temperature > 1233) {
        Tem_Flag = 1; // 如果ADC_MQ2的值超过阈值，设置MQ2_Flag为1
    } else {
        Tem_Flag = 0; // 如果ADC_MQ2的值没有超过阈值，保持MQ2_Flag为0或者将其设置为0
    }
}
void Hum_Check()
{
    if (humidity > 1233) {
        Hum_Flag = 1; // 如果ADC_MQ2的值超过阈值，设置MQ2_Flag为1
    } else {
        Hum_Flag = 0; // 如果ADC_MQ2的值没有超过阈值，保持MQ2_Flag为0或者将其设置为0
    }
}

void Over_Status(void)
{
    if(Tem_Flag ||Hum_Flag||MQ2_Flag  == 1)
    {
        BEEP_On();

        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        HAL_Delay(100);
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        HAL_Delay(100);
        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
        HAL_Delay(100);
    }


    if (Tem_Flag)
    {
        MOTO_Cotrol(1);
        FUN_Cotrol(1);
    }

    if (MQ2_Flag)
    {
        Steer_Angle(50);
    }

    MOTO_Cotrol(1);
    FUN_Cotrol(1);
    Steer_Angle(90);

}


void OLED_Show(void)
{

//    OLED_ShowString(0,0,"Mode: Auto",12, 0);
//    OLED_ShowString(0,2,"STEER: OFF",12,0);
//    OLED_ShowString(0,4,"FAN/MOTO: ON",12,0);
//    OLED_ShowString(0,6,"Tem:",16,0);
//    OLED_ShowString(0,8,"Hum:",16,0);
//    OLED_ShowString(0,10,"MQ2:",16,0);
//    OLED_ShowNum(5,6,temperature,4,4,0);
//    OLED_ShowNum(5,8,humidity,4,4,0);
//    OLED_ShowNum(6,6,temperature,4,4,0);
    if (Tem_Flag)
    {

    }

    if (MQ2_Flag)
    {

    }

    if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4))
    {
//        OLED_ShowString(0,4,"FAN/MOTO: ON ",12,0);
    } else{
//        OLED_ShowString(0,4,"FAN/MOTO: OFF",12,0);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3) {


//        OLED_Show();


        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);

    }
    else if (htim->Instance == TIM4) {

//        Hum_Check();
//        Tem_Check();
//        MQ2_Check();
//        Over_Status();


        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    }
}
