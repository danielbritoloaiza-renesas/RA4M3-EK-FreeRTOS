#ifndef BUTTON_EVENT_QUEUE_H_
#define BUTTON_EVENT_QUEUE_H_

#include "FreeRTOS.h"
#include "queue.h"

typedef enum
{
    BUTTON_EVENT_TOGGLE_RED_LED = 1
} button_event_t;

#define BUTTON_EVENT_QUEUE_LENGTH    (5U)

extern volatile uint32_t g_button_event_overflow_count;

QueueHandle_t button_event_queue_get(void);

#endif /* BUTTON_EVENT_QUEUE_H_ */
