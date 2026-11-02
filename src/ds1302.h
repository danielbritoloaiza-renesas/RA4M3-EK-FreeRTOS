#ifndef DS1302_H
#define DS1302_H
#include <stdint.h>

typedef struct {
    uint8_t seconds;  /* 0-59 */
    uint8_t minutes;  /* 0-59 */
    uint8_t hours;    /* 0-23 */
    uint8_t date;     /* 1-31 */
    uint8_t month;    /* 1-12 */
    uint8_t weekday;  /* DS1302 weekday register */
    uint8_t year;     /* 0-99 */
} ds1302_time_t;

extern ds1302_time_t g_time;

void ds1302_init(void);
void ds1302_read_time(ds1302_time_t *time);
void ds1302_set_time(const ds1302_time_t *time);

#endif
