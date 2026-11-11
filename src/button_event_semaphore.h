#ifndef BUTTON_EVENT_SEMAPHORE_H_
#define BUTTON_EVENT_SEMAPHORE_H_

#include "FreeRTOS.h"
#include "semphr.h"

#define BUTTON_EVENT_SEMAPHORE_MAX_COUNT    (5U)

extern volatile uint32_t g_button_semaphore_give_fail_count;

SemaphoreHandle_t button_event_semaphore_get(void);

#endif /* BUTTON_EVENT_SEMAPHORE_H_ */
