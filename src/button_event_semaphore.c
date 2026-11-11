#include "button_event_semaphore.h"

#include "task.h"

static StaticSemaphore_t g_button_event_semaphore_control;
static SemaphoreHandle_t g_button_event_semaphore;

volatile uint32_t g_button_semaphore_give_fail_count;

SemaphoreHandle_t button_event_semaphore_get(void)
{
    SemaphoreHandle_t semaphore;

    taskENTER_CRITICAL();
    if (NULL == g_button_event_semaphore)
    {
        g_button_event_semaphore = xSemaphoreCreateCountingStatic(BUTTON_EVENT_SEMAPHORE_MAX_COUNT,
                                                                  0U,
                                                                  &g_button_event_semaphore_control);
    }

    semaphore = g_button_event_semaphore;
    taskEXIT_CRITICAL();

    return semaphore;
}
