#include "Mcal_Ultrasonic.h"
#include "Rte.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

static volatile float s_distance_cm = 0.0f;
static volatile uint8_t s_capture_done = 0;
static uint32_t s_ic_val1 = 0;
static uint32_t s_ic_val2 = 0;
static uint8_t s_is_first_capture = 0;

void Mcal_Ultrasonic_Init(void)
{
    HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_2);
    HAL_TIM_Base_Start_IT(&htim3);
}

void Mcal_Ultrasonic_Trigger(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    for(volatile int i = 0; i < 32; i++);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

void Mcal_Ultrasonic_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for(;;)
    {
        if(s_capture_done)
        {
            s_capture_done = 0;

            /* Update data through RTE */
            Rte_Internal_UpdateUltrasonic(s_distance_cm, 1);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}

/* Timer Callbacks */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
    {
        if(s_is_first_capture == 0)
        {
            s_ic_val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
            s_is_first_capture = 1;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_FALLING);
        }
        else
        {
            s_ic_val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

            uint32_t time_diff;
            if(s_ic_val2 > s_ic_val1)
                time_diff = s_ic_val2 - s_ic_val1;
            else
                time_diff = (0xFFFFFFFF - s_ic_val1) + s_ic_val2;

            s_distance_cm = (float)time_diff * 0.0343f / 2.0f;

            if(s_distance_cm < 2.0f) s_distance_cm = 2.0f;
            if(s_distance_cm > 400.0f) s_distance_cm = 400.0f;

            s_capture_done = 1;
            s_is_first_capture = 0;

            __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_2, TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM3)
    {
        Mcal_Ultrasonic_Trigger();
    }
}
