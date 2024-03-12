//
// Created by Monee on 2024/3/11.
//
#include "Control.h"

volatile uint8_t Mode_Flag = 1;
volatile uint8_t Steer_Flag = 0;
volatile uint8_t Relay_Flag = 0;
volatile uint8_t MQ2_Flag = 0; // 用于指示MQ2传感器值是否超过阈值的标志位
volatile uint8_t Tem_Flag = 0;
volatile uint8_t Flash_Flag = 0;

volatile int temperature_threshold = 100;
volatile int MQ2_threshold = 100;


void MQ2_Check()
{
//    HAL_ADCEx_Calibration_Start(&hadc1);//ADC采样校准
//    ADC_MQ2 = ADC_IN_1();

    if (ADC_MQ2 > MQ2_threshold) {
        MQ2_Flag = 1; // 如果ADC_MQ2的值超过阈值，设置MQ2_Flag为1
    } else {
        MQ2_Flag = 0; // 如果ADC_MQ2的值没有超过阈值，保持MQ2_Flag为0或者将其设置为0
    }
}

void Tem_Check()
{
//

    if (temperature > temperature_threshold) {
        Tem_Flag = 1; // 如果ADC_MQ2的值超过阈值，设置MQ2_Flag为1
    } else {
        Tem_Flag = 0; // 如果ADC_MQ2的值没有超过阈值，保持MQ2_Flag为0或者将其设置为0
    }
}


void Update_System_Status() {

    MQ2_Check(); // 更新MQ2_Flag
    Tem_Check(); // 更新Tem_Flag

//        Steer_Angle(50);
//
//        Danger_Flag = 1;
    // 自动模式
    if (Mode_Flag == 1) {
        if (Tem_Flag || MQ2_Flag) {
            BEEP_On();
            Flash_Flag = 1; // 激活声光报警
            Relay_Cotrol(1);// 开启风扇和消防喷淋
            Steer_Angle(50);// 开启舵机

        } else {
            Relay_Cotrol(0);
            Relay_Cotrol(0);  // 关闭风扇和消防喷淋

        }
    }else{
        if (Tem_Flag || MQ2_Flag) {
            BEEP_On();
            Flash_Flag = 1; // 激活声光报警
            Relay_Cotrol(1);// 开启风扇和消防喷淋

        }
        else {
            //烟雾、温度下降后不执行操作不自动关闭
        }
    }

}


void OLED_Show(void)
{
//    printf("222\r\n");
    OLED_ShowNum(10,6,temperature,2,12,0);
    OLED_ShowNum(50,6,humidity,2,12,0);
    OLED_ShowNum(90,6,ADC_MQ2,3,12,0);

    if(Mode_Flag == 1){
        OLED_ShowString(0,0,"Mode:   Auto",12, 0);
    }else{
        OLED_ShowString(0,0,"Mode:   Manu",12, 0);
    }

    if(Steer_Flag == 1){
        OLED_ShowString(0,2,"STEER:    ON",12,0);
    }else{
        OLED_ShowString(0,2,"STEER:   OFF",12,0);
    }

    if(Relay_Flag == 1){
        OLED_ShowString(0,4,"FAN/MOTO: ON",12,0);
    }else{
        OLED_ShowString(0,4,"FAN/MOTO:OFF",12,0);
    }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    if(htim->Instance == TIM4) {
//        Update_System_Status();
//        HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
    }

}
