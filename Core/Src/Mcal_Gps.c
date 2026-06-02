#include "Mcal_Gps.h"
#include "Rte.h"
#include <string.h>
#include <stdlib.h>

extern UART_HandleTypeDef huart1;

static uint8_t s_gps_rx_buffer[GPS_BUFFER_SIZE];
static uint8_t s_gps_process_buffer[GPS_BUFFER_SIZE];
static volatile uint8_t s_gps_data_ready = 0;

void Mcal_Gps_Init(void)
{
    HAL_UART_Receive_DMA(&huart1, s_gps_rx_buffer, GPS_BUFFER_SIZE);
}

void Mcal_Gps_ParseNMEA(void)
{
    char *ptr = strstr((char*)s_gps_process_buffer, "$GPGGA");
    if (ptr == NULL) ptr = strstr((char*)s_gps_process_buffer, "$GNGGA");
    if (ptr == NULL) return;

    char temp_buffer[256];
    char *end = strchr(ptr, '\n');
    if (end == NULL || (end - ptr) > 255) return;

    memcpy(temp_buffer, ptr, end - ptr);
    temp_buffer[end - ptr] = '\0';

    char raw_lat[20] = {0};
    char raw_lon[20] = {0};
    char lat_dir = 0, lon_dir = 0;
    char quality[5] = {0}, sats[5] = {0}, altitude[15] = {0};

    char *token = strtok(temp_buffer, ",");
    int index = 0;

    while (token != NULL && index < 15)
    {
        if (index == 2 && strlen(token) > 0) strcpy(raw_lat, token);
        if (index == 3 && strlen(token) > 0) lat_dir = token[0];
        if (index == 4 && strlen(token) > 0) strcpy(raw_lon, token);
        if (index == 5 && strlen(token) > 0) lon_dir = token[0];
        if (index == 6 && strlen(token) > 0) strcpy(quality, token);
        if (index == 7 && strlen(token) > 0) strcpy(sats, token);
        if (index == 9 && strlen(token) > 0) strcpy(altitude, token);

        index++;
        token = strtok(NULL, ",");
    }

    if (strlen(raw_lat) < 3 || strlen(raw_lon) < 3) return;

    double lat_val = atof(raw_lat);
    double lon_val = atof(raw_lon);
    if(lat_val == 0.0 || lon_val == 0.0) return;

    double lat_deg = (int)(lat_val / 100);
    double lat_min = lat_val - (lat_deg * 100);
    double lat_decimal = lat_deg + (lat_min / 60.0);

    double lon_deg = (int)(lon_val / 100);
    double lon_min = lon_val - (lon_deg * 100);
    double lon_decimal = lon_deg + (lon_min / 60.0);

    if (lat_dir == 'S') lat_decimal = -lat_decimal;
    if (lon_dir == 'W') lon_decimal = -lon_decimal;

    /* Update RTE data */
    Rte_GpsData_t gpsData;
    gpsData.latitude = lat_decimal;
    gpsData.longitude = lon_decimal;
    gpsData.altitude = atof(altitude);
    gpsData.satellites = atoi(sats);
    gpsData.fix_quality = atoi(quality);
    gpsData.valid = (gpsData.fix_quality > 0) ? 1 : 0;

    Rte_Internal_UpdateGps(&gpsData);
}

void Mcal_Gps_Task(void *pvParameters)
{
    for(;;)
    {
        if(s_gps_data_ready)
        {
            s_gps_data_ready = 0;
            Mcal_Gps_ParseNMEA();
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/* UART Callbacks */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        memcpy(s_gps_process_buffer, s_gps_rx_buffer, GPS_BUFFER_SIZE);
        s_gps_data_ready = 1;
    }
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        memcpy(s_gps_process_buffer, s_gps_rx_buffer, GPS_BUFFER_SIZE / 2);
        s_gps_data_ready = 1;
    }
}
