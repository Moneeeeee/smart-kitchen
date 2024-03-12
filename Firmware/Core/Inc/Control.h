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
extern volatile uint8_t Hum_Flag;
extern volatile uint8_t Flash_Flag;
extern volatile uint8_t Danger_Flag;

extern volatile int temperature_threshold;
extern volatile int humidity_threshold ;
extern volatile int MQ2_threshold ;

void MQ2_Check(void);
void Tem_Check(void);
void Hum_Check(void);
void Over_Status(void);
void OLED_Show(void);
void Update_System_Status(void);
#endif //FIRMWARE_CONTROL_H
