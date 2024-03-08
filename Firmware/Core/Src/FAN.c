//
// Created by Monee on 2024/3/8.
//

#include "FAN.h"
/**
  * @brief 控制继电器的开关状态
  * @param state: 如果为1，则开启继电器；如果为0，则关闭继电器。
  * @retval None
  */
void FAN_Control(uint8_t state) {
    if (state) {
        // Turn on the relay
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    } else {
        // Turn off the relay
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    }
}

