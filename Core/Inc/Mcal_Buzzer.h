#ifndef MCAL_BUZZER_H
#define MCAL_BUZZER_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void Mcal_Buzzer_Init(void);
void Mcal_Buzzer_Task(void *pvParameters);
void Mcal_Buzzer_SetFrequency(uint16_t frequency);
void Mcal_Buzzer_Enable(uint8_t enable);

#endif
