#include "bsp_api.h"
#include "systemview_lock.h"

static uint32_t s_lock_depth;
static uint32_t s_saved_primask;

void systemview_lock(void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    if (0U == s_lock_depth)
    {
        s_saved_primask = primask;
    }

    s_lock_depth++;
}

void systemview_unlock(void)
{
    if (0U != s_lock_depth)
    {
        s_lock_depth--;
        if (0U == s_lock_depth)
        {
            __set_PRIMASK(s_saved_primask);
        }
    }
}
