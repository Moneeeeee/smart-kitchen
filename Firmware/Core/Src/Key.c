//
// Created by Monee on 2024/3/11.

#include "Key.h"





void Key_Scan()
{

}

__weak void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY1_Pin) {
        //切换模式
        Mode_Flag = 1-Mode_Flag;
    }
    else if (GPIO_Pin == KEY2_Pin) {
        if (Mode_Flag == 0)        //手动模式下
        {
            Relay_Cotrol(0);
            Relay_Flag = 0;
        }


    }
    else if (GPIO_Pin == KEY3_Pin) {
        //关闭舵机
        Steer_Angle(90);
    }
}