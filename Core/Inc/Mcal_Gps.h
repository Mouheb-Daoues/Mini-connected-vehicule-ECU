#ifndef MCAL_GPS_H
#define MCAL_GPS_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

#define GPS_BUFFER_SIZE 1024

void Mcal_Gps_Init(void);
void Mcal_Gps_Task(void *pvParameters);
void Mcal_Gps_ParseNMEA(void);

#endif
