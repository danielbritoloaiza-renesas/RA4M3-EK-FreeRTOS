/* generated thread header file - do not edit */
#ifndef RED_THREAD_H_
#define RED_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void red_thread_entry(void * pvParameters);
                #else
extern void red_thread_entry(void *pvParameters);
#endif
FSP_HEADER
FSP_FOOTER
#endif /* RED_THREAD_H_ */
