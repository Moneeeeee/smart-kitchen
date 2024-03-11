//
// Created by Monee on 2024/3/8.
//

#ifndef FIRMWARE_BEEP_H
#define FIRMWARE_BEEP_H
#include "main.h"

void Buzzer_Beep(uint32_t onTimeMs, uint32_t offTimeMs, uint8_t repetitions);
void BEEP_Init(void);

void BEEP_On(void);

void BEEP_Off(void);

#endif //FIRMWARE_BEEP_H
