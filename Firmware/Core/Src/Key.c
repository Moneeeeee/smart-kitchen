//
// Created by Monee on 2024/3/11.

#include "Key.h"
#include "Control.h"
#include "stdint.h"
#include "string.h"
#include <stdio.h>

__weak void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {


    if (GPIO_Pin == KEY1_Pin && HAL_GPIO_ReadPin(GPIOB, KEY1_Pin) == GPIO_PIN_RESET) {
        Mode_Flag = (Mode_Flag == 1) ? 0 : 1;
    }
        // 检测按键2
    else if (GPIO_Pin == KEY2_Pin && HAL_GPIO_ReadPin(GPIOB, KEY2_Pin) == GPIO_PIN_RESET) {
        if (Mode_Flag == 0) {
            // 手动模式下，开启或关闭舵机
            if (Steer_Flag == 0) {
                Steer_Angle(50); // 开启舵机
                Steer_Flag = 1;
            } else if (Steer_Flag == 1) {
                Steer_Angle(90); // 开启舵机 // 关闭舵机
                Steer_Flag = 0;
            }
        }

        else if(Mode_Flag ==1){
            Invert_Flag = (Invert_Flag == 1) ? 0 : 1;
        }

    }
        // 检测按键3
    else if (GPIO_Pin == KEY3_Pin && HAL_GPIO_ReadPin(GPIOB, KEY3_Pin) == GPIO_PIN_RESET) {
        if (Mode_Flag == 0) {
            // 开启或关闭继电器
            if (FUN_Flag == 0) {
                FUN_Cotrol(1); // 开启继电
                FUN_Flag = 1;
            } else {
                FUN_Cotrol(0); // 开启继电
                FUN_Flag = 0;
            }

        }
        else if(Mode_Flag == 1){
            if(Invert_Flag == 1)
            {
                MQ2_threshold++;
            }
            else if(Invert_Flag == 0)
            {
                MQ2_threshold--;
            }
        }

    } else if (GPIO_Pin == KEY4_Pin && HAL_GPIO_ReadPin(GPIOB, KEY4_Pin) == GPIO_PIN_RESET) {

        if (Mode_Flag == 0){
            // 开启或关闭继电器
            if (Water_Flag == 0) {
                Water_Cotrol(1); // 开启继电
                Water_Flag = 1;
            } else {
                Water_Cotrol(0); // 开启继电
                Water_Flag = 0;
            }
        }

        else if(Mode_Flag == 1){
            if(Invert_Flag == 1)
            {
                tem_threshold++;
            }
            else if(Invert_Flag == 0)
            {
                tem_threshold--;
            }
        }
    }

}