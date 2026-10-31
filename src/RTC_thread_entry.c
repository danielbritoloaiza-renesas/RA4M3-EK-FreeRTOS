#include "RTC_thread.h"
/* RTC Thread entry function */
/* pvParameters contains TaskHandle_t */
void RTC_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);
    /* Holds level to set for pins */
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    /* TODO: add your own code here */
    while (1)
    {
        R_BSP_PinAccessEnable();
        R_BSP_PinWrite((bsp_io_port_pin_t) (uint16_t) BSP_IO_PORT_05_PIN_02, pin_level);
        R_BSP_PinWrite((bsp_io_port_pin_t) (uint16_t) BSP_IO_PORT_05_PIN_03, pin_level);
        R_BSP_PinWrite((bsp_io_port_pin_t) (uint16_t) BSP_IO_PORT_05_PIN_04, pin_level);


        /* Protect PFS registers */
        R_BSP_PinAccessDisable();
        /* Toggle level for next write */
        if (BSP_IO_LEVEL_LOW == pin_level)
        {
            pin_level = BSP_IO_LEVEL_HIGH;
        }
        else
        {
            pin_level = BSP_IO_LEVEL_LOW;
        }
        xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(10));
    }
}
