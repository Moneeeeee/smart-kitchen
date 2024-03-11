//
// Created by Monee on 2024/3/11.
//
#include "Steer.h"


void Steer_Angle(uint8_t angle)
{
    __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,angle+ 50); //相当于一个周期内（20ms）有0.5ms高脉冲
}


void Steer_Init(void)
{
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
    Steer_Angle(90);
}

