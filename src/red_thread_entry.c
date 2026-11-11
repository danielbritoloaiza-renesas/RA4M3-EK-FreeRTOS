/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include <red_thread.h>
#include "button_event_semaphore.h"

extern bsp_leds_t g_bsp_leds;

/* Red Thread entry function */
void red_thread_entry (void * pvParameters)
{
    FSP_PARAMETER_NOT_USED(pvParameters);

    /* LED type structure */
    bsp_leds_t leds = g_bsp_leds;

    /* Wake up 2nd core if this is first core and we are inside a multicore project. */
#if (0 == _RA_CORE) && (1 == BSP_MULTICORE_PROJECT) && !BSP_TZ_NONSECURE_BUILD
    R_BSP_SecondaryCoreStart();
#endif

    /* If this board has no LEDs then trap here */
    if (0 == leds.led_count)
    {
        while (1)
        {
            ;                          // There are no LEDs on this board
        }
    }

    SemaphoreHandle_t event_semaphore = button_event_semaphore_get();
    if (NULL == event_semaphore)
    {
        BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
    }

    bool led_on = false;

    R_BSP_PinAccessEnable();
    R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[2], BSP_IO_LEVEL_HIGH);
    R_BSP_PinAccessDisable();

    while (1)
    {
        if (pdFALSE == xSemaphoreTake(event_semaphore, portMAX_DELAY))
        {
            BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
        }

        led_on = !led_on;

        R_BSP_PinAccessEnable();
        R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[2],
                       led_on ? BSP_IO_LEVEL_LOW : BSP_IO_LEVEL_HIGH);
        R_BSP_PinAccessDisable();

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
