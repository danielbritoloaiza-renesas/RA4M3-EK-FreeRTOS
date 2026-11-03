/*********************************************************************
* SEGGER RTT user configuration for the RA4M3 FreeRTOS application.
**********************************************************************
*/

#ifndef SEGGER_RTT_CONF_H
#define SEGGER_RTT_CONF_H

#include "systemview_lock.h"

#define RTT_USE_ASM       0
#define SEGGER_RTT_LOCK()   systemview_lock()
#define SEGGER_RTT_UNLOCK() systemview_unlock()

#endif
