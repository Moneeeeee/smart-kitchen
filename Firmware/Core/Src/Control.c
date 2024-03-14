//
// Created by Monee on 2024/3/11.
//
#include "Control.h"
#include "adc.h"
#include "DHT11.h"
#include "BEEP.h"
#include "tim.h"
#include "OLED.h"
volatile uint8_t Mode_Flag = 1;
volatile uint8_t Steer_Flag = 0;
volatile uint8_t Relay_Flag = 0;
volatile uint8_t MQ2_Flag = 0; // 用于指示MQ2传感器值是否超过阈值的标志位
volatile uint8_t Tem_Flag = 0;
volatile uint8_t Flash_Flag = 0;

volatile uint8_t temperature_threshold = 100;
volatile uint8_t MQ2_threshold = 100;

uint16_t ADC_MQ2;




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

uint16_t ADC_IN_1(void) //ADC采集程序
{
    HAL_ADC_Start(&hadc1);//开始ADC采集
    HAL_ADC_PollForConversion(&hadc1,100);//等待采集结束
    if(HAL_IS_BIT_SET(HAL_ADC_GetState(&hadc1), HAL_ADC_STATE_REG_EOC))//读取ADC完成标志位
    {
        return HAL_ADC_GetValue(&hadc1);//读出ADC数值
    }
    return 0;
}


void MQ2_Check()
{
    HAL_ADCEx_Calibration_Start(&hadc1);
    ADC_MQ2 = ADC_IN_1();
    MQ2_Flag = (ADC_MQ2 > MQ2_threshold) ? 1 : 0;
}

void Tem_Check()
{
    DHT11_Read_Data(&temperature, &humidity);
    Tem_Flag = (temperature > temperature_threshold) ? 1 : 0;
}


void Relay_Cotrol(uint8_t state)
{
    if (state)
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
        Relay_Flag = 1;

    }
    else
    {
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
        Relay_Flag = 0;


    }

}


void Update_System_Status() {
    MQ2_Check();
    Tem_Check();

    if (Mode_Flag == 1) {
        if (Tem_Flag || MQ2_Flag) {
            BEEP_On();
            Flash_Flag = 1;
            Relay_Cotrol(1);
            Steer_Angle(50);
        } else {
            Relay_Cotrol(0);
        }
    } else {
        if (Tem_Flag || MQ2_Flag) {
            BEEP_On();
            Flash_Flag = 1;
            Relay_Cotrol(1);
        }
    }
}


void OLED_Show(void)
{
    OLED_ShowNum(10, 6, temperature, 2, 12, 0);
    OLED_ShowNum(50, 6, humidity, 2, 12, 0);
    OLED_ShowNum(90, 6, ADC_MQ2, 3, 12, 0);

    OLED_ShowString(0, 0, Mode_Flag == 1 ? "Mode:   Auto" : "Mode:   Manu", 12, 0);
    OLED_ShowString(0, 2, Steer_Flag == 1 ? "STEER:    ON" : "STEER:   OFF", 12, 0);
    OLED_ShowString(0, 4, Relay_Flag == 1 ? "FAN/MOTO: ON" : "FAN/MOTO:OFF", 12, 0);
}

