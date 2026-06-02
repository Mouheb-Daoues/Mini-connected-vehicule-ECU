#include "App_LocationManager.h"
#include "Rte.h"

void App_LocationManager_Init(void)
{
    /* Nothing to initialize */
}

void App_LocationManager_Task(void *pvParameters)
{
    Rte_GpsData_t gpsData;

    for(;;)
    {
        if(Rte_Read_GpsPosition(&gpsData) == E_OK && gpsData.valid)
        {
            /* GPS data is valid - can be used for logging, geofencing, etc. */
            /* For now, just monitoring. Add your application logic here. */
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
