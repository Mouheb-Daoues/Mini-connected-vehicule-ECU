#ifndef APP_LOCATION_MANAGER_H
#define APP_LOCATION_MANAGER_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

void App_LocationManager_Init(void);
void App_LocationManager_Task(void *pvParameters);

#endif
