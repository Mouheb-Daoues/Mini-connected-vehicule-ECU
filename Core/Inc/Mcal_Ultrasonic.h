#ifndef MCAL_ULTRASONIC_H
#define MCAL_ULTRASONIC_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

void Mcal_Ultrasonic_Init(void);
void Mcal_Ultrasonic_Task(void *pvParameters);
void Mcal_Ultrasonic_Trigger(void);

#endif
