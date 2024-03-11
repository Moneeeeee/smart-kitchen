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


void MQ2_Check(void);
void Tem_Check(void);
void Hum_Check(void);
void Over_Status(void);
void OLED_Show(void);

#endif //FIRMWARE_CONTROL_H
