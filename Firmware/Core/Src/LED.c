//
// Created by Monee on 2024/3/8.
//

#include "LED.h"

void LED_ON(void)
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
}

void LED_OFF(void)
{
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
}

void LED_Blink(uint16_t delay) {
    // Toggle the LED
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);

    // Wait for 'delay' milliseconds
    HAL_Delay(delay);
}
