#ifndef APP_PROXIMITY_MANAGER_H
#define APP_PROXIMITY_MANAGER_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

void App_ProximityManager_Init(void);
void App_ProximityManager_Task(void *pvParameters);

#endif
