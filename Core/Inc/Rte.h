#ifndef RTE_H
#define RTE_H

#include "main.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

/* AUTOSAR Standard Types */
typedef uint8_t Std_ReturnType;
#define E_OK        ((Std_ReturnType)0x00U)
#define E_NOT_OK    ((Std_ReturnType)0x01U)

/* Type definitions */
typedef struct {
    float distance_cm;
    uint32_t timestamp;
    uint8_t valid;
} Rte_UltrasonicData_t;

typedef struct {
    double latitude;
    double longitude;
    float altitude;
    uint8_t satellites;
    uint8_t fix_quality;
    uint8_t valid;
} Rte_GpsData_t;

typedef enum {
    ZONE_SAFE = 0,
    ZONE_WARNING,
    ZONE_CAUTION,
    ZONE_DANGER,
    ZONE_CRITICAL
} Rte_ProximityZone_t;

typedef struct {
    uint16_t frequency;
    uint8_t enable;
} BuzzerCmd_t;

/* RTE API - Utilisé par ASW pour accéder aux données BSW */
Std_ReturnType Rte_Read_UltrasonicDistance(Rte_UltrasonicData_t *data);
Std_ReturnType Rte_Read_GpsPosition(Rte_GpsData_t *data);
Std_ReturnType Rte_Write_BuzzerControl(uint16_t frequency, uint8_t enable);
void Rte_Init(void);

/* Internal functions for BSW layer */
void Rte_Internal_UpdateUltrasonic(float distance, uint8_t valid);
void Rte_Internal_UpdateGps(Rte_GpsData_t *gpsData);

/* Mutex et synchronisation */
extern SemaphoreHandle_t xMutex_UltrasonicData;
extern SemaphoreHandle_t xMutex_GpsData;
extern QueueHandle_t xQueue_BuzzerCmd;

#endif /* RTE_H */
