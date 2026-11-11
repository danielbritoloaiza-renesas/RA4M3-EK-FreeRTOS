#include "button1_thread0.h"
#include "button_event_semaphore.h"

                /* Button Thread entry function */
                /* pvParameters contains TaskHandle_t */
                void button1_thread0_entry(void * pvParameters)
                {
                    FSP_PARAMETER_NOT_USED(pvParameters);

                    SemaphoreHandle_t event_semaphore = button_event_semaphore_get();
                    if (NULL == event_semaphore)
                    {
                        BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
                    }

                    bsp_io_level_t level;
                    BaseType_t was_pressed = pdFALSE;

                    while (1)
                    {
                        fsp_err_t err = g_ioport.p_api->pinRead(g_ioport.p_ctrl, SW1, &level);
                        if (FSP_SUCCESS != err)
                        {
                            BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
                        }

                        BaseType_t is_pressed = (BSP_IO_LEVEL_LOW == level) ? pdTRUE : pdFALSE;

                        if (is_pressed != was_pressed)
                        {
                            vTaskDelay(pdMS_TO_TICKS(30));

                            err = g_ioport.p_api->pinRead(g_ioport.p_ctrl, SW1, &level);
                            if (FSP_SUCCESS != err)
                            {
                                BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
                            }

                            is_pressed = (BSP_IO_LEVEL_LOW == level) ? pdTRUE : pdFALSE;

                            if (is_pressed != was_pressed)
                            {
                                was_pressed = is_pressed;

                                if (pdTRUE == is_pressed)
                                {
                                    if (pdFALSE == xSemaphoreGive(event_semaphore))
                                    {
                                        g_button_semaphore_give_fail_count++;
                                    }
                                }
                            }
                        }

                        vTaskDelay(pdMS_TO_TICKS(10));
                    }
                }
