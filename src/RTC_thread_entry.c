#include "RTC_thread.h"
#include "ds1302.h" 

ds1302_time_t g_time;

/* RTC Thread entry function */
/* pvParameters contains TaskHandle_t */
void RTC_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);
    TickType_t xLastWakeTime = xTaskGetTickCount();
    ds1302_init();

    while (1)
    {
        ds1302_read_time(&g_time);
        xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));
    }
}
