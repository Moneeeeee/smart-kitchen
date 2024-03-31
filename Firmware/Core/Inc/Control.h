//
// Created by Monee on 2024/3/11.
//

#ifndef FIRMWARE_CONTROL_H
#define FIRMWARE_CONTROL_H


#include "main.h"

extern volatile uint8_t Mode_Flag;
extern volatile uint8_t Steer_Flag;
extern volatile uint8_t Relay_Flag;
extern volatile uint8_t MQ2_Flag;
extern volatile uint8_t Tem_Flag;
extern volatile uint8_t Flash_Flag;
extern volatile uint8_t FUN_Flag ;
extern volatile uint8_t Water_Flag;
extern volatile uint8_t tem_threshold ;
extern volatile uint16_t MQ2_threshold ;
extern volatile uint16_t MQ4_threshold ;
extern volatile uint8_t Invert_Flag;
extern uint16_t ADC_MQ2;
extern uint16_t ADC_MQ4;
void MQ_Check(void);
void Tem_Check(void);
void Steer_Angle(uint8_t angle);
void Steer_Init(void);
void OLED_Show(void);
void Update_System_Status(void);
void Relay_Cotrol(uint8_t state);
void FUN_Cotrol(uint8_t state);
void Water_Cotrol(uint8_t state);
uint16_t ADC_IN_1(void);

int16_t ADC_Read_Channel(ADC_HandleTypeDef* hadc, uint32_t channel) ;
#endif //FIRMWARE_CONTROL_H
