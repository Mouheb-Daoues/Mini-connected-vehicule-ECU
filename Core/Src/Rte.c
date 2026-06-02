#include "Rte.h"

/* Shared data protected by mutex */
static Rte_UltrasonicData_t g_UltrasonicData = {0};
static Rte_GpsData_t g_GpsData = {0};

/* Synchronization primitives */
SemaphoreHandle_t xMutex_UltrasonicData = NULL;
SemaphoreHandle_t xMutex_GpsData = NULL;
QueueHandle_t xQueue_BuzzerCmd = NULL;


void Rte_Init(void)
{
    /* Create mutexes */
    xMutex_UltrasonicData = xSemaphoreCreateMutex();
    xMutex_GpsData = xSemaphoreCreateMutex();

    /* Create queues */
    xQueue_BuzzerCmd = xQueueCreate(5, sizeof(BuzzerCmd_t));
}

Std_ReturnType Rte_Read_UltrasonicDistance(Rte_UltrasonicData_t *data)
{
    if (xSemaphoreTake(xMutex_UltrasonicData, pdMS_TO_TICKS(100)) == pdTRUE)
    {
        *data = g_UltrasonicData;
        xSemaphoreGive(xMutex_UltrasonicData);
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Rte_Read_GpsPosition(Rte_GpsData_t *data)
{
    if (xSemaphoreTake(xMutex_GpsData, pdMS_TO_TICKS(100)) == pdTRUE)
    {
        *data = g_GpsData;
        xSemaphoreGive(xMutex_GpsData);
        return E_OK;
    }
    return E_NOT_OK;
}

Std_ReturnType Rte_Write_BuzzerControl(uint16_t frequency, uint8_t enable)
{
    BuzzerCmd_t cmd = {frequency, enable};
    if (xQueueSend(xQueue_BuzzerCmd, &cmd, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        return E_OK;
    }
    return E_NOT_OK;
}

/* Internal functions for BSW layer to update data */
void Rte_Internal_UpdateUltrasonic(float distance, uint8_t valid)
{
    if (xSemaphoreTake(xMutex_UltrasonicData, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        g_UltrasonicData.distance_cm = distance;
        g_UltrasonicData.timestamp = HAL_GetTick();
        g_UltrasonicData.valid = valid;
        xSemaphoreGive(xMutex_UltrasonicData);
    }
}

void Rte_Internal_UpdateGps(Rte_GpsData_t *gpsData)
{
    if (xSemaphoreTake(xMutex_GpsData, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        g_GpsData = *gpsData;
        xSemaphoreGive(xMutex_GpsData);
    }
}
