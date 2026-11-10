#include "button_event_queue.h"

#include "task.h"

static StaticQueue_t g_button_event_queue_control;
static uint8_t g_button_event_queue_storage[BUTTON_EVENT_QUEUE_LENGTH * sizeof(button_event_t)];
static QueueHandle_t g_button_event_queue;

volatile uint32_t g_button_event_overflow_count;

QueueHandle_t button_event_queue_get(void)
{
    QueueHandle_t queue;

    taskENTER_CRITICAL();
    if (NULL == g_button_event_queue)
    {
        g_button_event_queue = xQueueCreateStatic(BUTTON_EVENT_QUEUE_LENGTH,
                                                  sizeof(button_event_t),
                                                  g_button_event_queue_storage,
                                                  &g_button_event_queue_control);
    }

    queue = g_button_event_queue;
    taskEXIT_CRITICAL();

    return queue;
}
