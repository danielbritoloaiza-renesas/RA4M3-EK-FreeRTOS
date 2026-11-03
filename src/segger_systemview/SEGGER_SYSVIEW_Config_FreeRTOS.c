#include "bsp_api.h"
#include "FreeRTOS.h"
#include "SEGGER_SYSVIEW.h"

extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;

#define SYSVIEW_APP_NAME        "FreeRTOS_EKRA4M3"
#define SYSVIEW_DEVICE_NAME     "Renesas EK-RA4M3"
#define SYSVIEW_TIMESTAMP_FREQ  (configCPU_CLOCK_HZ)
#define SYSVIEW_CPU_FREQ        (configCPU_CLOCK_HZ)
#define SYSVIEW_RAM_BASE        (0x20000000UL)

static void systemview_send_description(void)
{
    SEGGER_SYSVIEW_SendSysDesc("N=" SYSVIEW_APP_NAME ",D=" SYSVIEW_DEVICE_NAME ",O=FreeRTOS");
    SEGGER_SYSVIEW_SendSysDesc("I#15=SysTick");
}

void SEGGER_SYSVIEW_Conf(void)
{
    static bool initialized = false;

    if (initialized)
    {
        return;
    }
    initialized = true;

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0U;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    SEGGER_SYSVIEW_Init(SYSVIEW_TIMESTAMP_FREQ,
                        SYSVIEW_CPU_FREQ,
                        &SYSVIEW_X_OS_TraceAPI,
                        systemview_send_description);
    SEGGER_SYSVIEW_SetRAMBase(SYSVIEW_RAM_BASE);

#if SEGGER_SYSVIEW_START_ON_INIT
    SEGGER_SYSVIEW_Start();
#endif
}
