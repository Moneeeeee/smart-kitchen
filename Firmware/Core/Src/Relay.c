//
// Created by Monee on 2024/3/11.
//
#include "Relay.h"

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

