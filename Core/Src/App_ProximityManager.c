#include "App_ProximityManager.h"
#include "Rte.h"

#define ZONE_SAFE_DIST      50.0f
#define ZONE_WARNING_DIST   30.0f
#define ZONE_CAUTION_DIST   15.0f
#define ZONE_DANGER_DIST    5.0f

typedef struct {
    uint32_t period_ms;
    uint32_t duration_ms;
    uint16_t frequency;
} BuzzerPattern_t;

static const BuzzerPattern_t s_BuzzerPatterns[] = {
    [ZONE_SAFE]     = {0,    0,    0},
    [ZONE_WARNING]  = {1000, 100,  500},
    [ZONE_CAUTION]  = {500,  100,  1000},
    [ZONE_DANGER]   = {250,  80,   1500},
    [ZONE_CRITICAL] = {0,    0,    2000}  // Continuous
};

void App_ProximityManager_Init(void)
{
    /* Nothing to initialize */
}

Rte_ProximityZone_t App_GetProximityZone(float distance)
{
    if(distance > ZONE_SAFE_DIST) return ZONE_SAFE;
    if(distance > ZONE_WARNING_DIST) return ZONE_WARNING;
    if(distance > ZONE_CAUTION_DIST) return ZONE_CAUTION;
    if(distance > ZONE_DANGER_DIST) return ZONE_DANGER;
    return ZONE_CRITICAL;
}

void App_ProximityManager_Task(void *pvParameters)
{
    Rte_UltrasonicData_t ultrasonicData;
    Rte_ProximityZone_t currentZone = ZONE_SAFE;
    uint32_t lastToggleTime = 0;
    uint8_t buzzerOn = 0;

    for(;;)
    {
        if(Rte_Read_UltrasonicDistance(&ultrasonicData) == E_OK && ultrasonicData.valid)
        {
            Rte_ProximityZone_t newZone = App_GetProximityZone(ultrasonicData.distance_cm);

            if(newZone != currentZone)
            {
                currentZone = newZone;
                lastToggleTime = HAL_GetTick();
                buzzerOn = 0;
            }

            const BuzzerPattern_t *pattern = &s_BuzzerPatterns[currentZone];
            uint32_t currentTime = HAL_GetTick();

            if(currentZone == ZONE_CRITICAL)
            {
                /* Continuous beep */
                Rte_Write_BuzzerControl(pattern->frequency, 1);
            }
            else if(currentZone == ZONE_SAFE)
            {
                /* No beep */
                Rte_Write_BuzzerControl(0, 0);
            }
            else
            {
                /* Intermittent beep */
                if(!buzzerOn && (currentTime - lastToggleTime >= pattern->period_ms))
                {
                    Rte_Write_BuzzerControl(pattern->frequency, 1);
                    buzzerOn = 1;
                    lastToggleTime = currentTime;
                }
                else if(buzzerOn && (currentTime - lastToggleTime >= pattern->duration_ms))
                {
                    Rte_Write_BuzzerControl(0, 0);
                    buzzerOn = 0;
                }
            }
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
