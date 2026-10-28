#include "blue_thread0.h"

extern bsp_leds_t g_bsp_leds;

/* Blue Thread entry function */
/* pvParameters contains TaskHandle_t */
void blue_thread0_entry(void *pvParameters)
{
 FSP_PARAMETER_NOT_USED(pvParameters);
    TickType_t xLastWakeTime = xTaskGetTickCount();

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

    /* Holds level to set for pins */
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;

    while (1)
    {
        /* Enable access to the PFS registers. If using r_ioport module then register protection is automatically
         * handled. This code uses BSP IO functions to show how it is used.
         */
        R_BSP_PinAccessEnable();

#if BSP_NUMBER_OF_CORES == 1

        /* Update all board LEDs */

        /* Write to this pin */
        R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[0], pin_level);

#else

        /* Update LED that is at the index of this core. */
        R_BSP_PinWrite((bsp_io_port_pin_t) leds.p_leds[_RA_CORE], pin_level);
#endif

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

        xTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(100));
    }
}
