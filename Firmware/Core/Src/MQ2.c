//
// Created by Monee on 2024/3/8.
//

#include "MQ2.h"

uint16_t ADC_MQ2;

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