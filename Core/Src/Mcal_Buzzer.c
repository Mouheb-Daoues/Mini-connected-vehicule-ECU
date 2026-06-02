#include "Mcal_Buzzer.h"
#include "Rte.h"

extern TIM_HandleTypeDef htim4;

void Mcal_Buzzer_Init(void)
{
    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
}

void Mcal_Buzzer_SetFrequency(uint16_t frequency)
{
    if(frequency < 100) frequency = 100;
    if(frequency > 5000) frequency = 5000;

    uint32_t period = (16000000 / (16 * frequency)) - 1;

    HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
    __HAL_TIM_SET_AUTORELOAD(&htim4, period);
    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, period / 2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
}

void Mcal_Buzzer_Enable(uint8_t enable)
{
    if(enable)
        HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    else
        HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
}

void Mcal_Buzzer_Task(void *pvParameters)
{
    BuzzerCmd_t cmd;

    for(;;)
    {
        if(xQueueReceive(xQueue_BuzzerCmd, &cmd, portMAX_DELAY) == pdTRUE)
        {
            if(cmd.enable)
            {
                Mcal_Buzzer_SetFrequency(cmd.frequency);
                Mcal_Buzzer_Enable(1);
            }
            else
            {
                Mcal_Buzzer_Enable(0);
            }
        }
    }
}
