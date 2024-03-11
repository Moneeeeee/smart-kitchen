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
        if (Mode_Flag)
        {
            OLED_ShowString(0,0,"Mode: Auto  ",16, 0);//Mode_Flag== 1 自动
        } else{
            OLED_ShowString(0,0,"Mode: Manual",16, 0);
        }


    }
    else if (GPIO_Pin == KEY2_Pin) {
        if (Mode_Flag == 0)        //手动模式下
        {
            MOTO_Cotrol(0);  //关闭继电器
            FUN_Cotrol(0);
            Relay_Flag = 0;
            if (Relay_Flag == 0)
            {
                OLED_ShowString(0,6,"FAN/MOTO: OFF",16,0);
            }
        }


    }
    else if (GPIO_Pin == KEY3_Pin) {
        //关闭舵机
        Steer_Angle(90);
        Steer_Flag = 0;
        if (Steer_Flag == 0)
        {
            OLED_ShowString(0,3,"STEER: OFF",16,0);
        }

    }
}