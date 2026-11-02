#include "bsp_api.h"
#include "r_ioport.h"
#include "ds1302.h"

#define DS1302_CE_PIN       ((bsp_io_port_pin_t) BSP_IO_PORT_05_PIN_02)
#define DS1302_IO_PIN       ((bsp_io_port_pin_t) BSP_IO_PORT_05_PIN_03)
#define DS1302_SCLK_PIN     ((bsp_io_port_pin_t) BSP_IO_PORT_05_PIN_04)

#define DS1302_SECONDS      (0x80U)
#define DS1302_MINUTES      (0x82U)
#define DS1302_HOURS        (0x84U)
#define DS1302_DATE         (0x86U)
#define DS1302_MONTH        (0x88U)
#define DS1302_WEEKDAY      (0x8AU)
#define DS1302_YEAR         (0x8CU)
#define DS1302_WRITE_PROTECT (0x8EU)
#define DS1302_BURST_READ   (0xBFU)

static void ds1302_delay(void)
{
    R_BSP_SoftwareDelay(2U, BSP_DELAY_UNITS_MICROSECONDS);
}

static void ds1302_io_output(void)
{
    R_BSP_PinCfg(DS1302_IO_PIN,
                 IOPORT_CFG_PORT_DIRECTION_OUTPUT | IOPORT_CFG_PORT_OUTPUT_HIGH);
}

static void ds1302_io_input(void)
{
    R_BSP_PinCfg(DS1302_IO_PIN, IOPORT_CFG_PORT_DIRECTION_INPUT);
}

static void ds1302_write_byte(uint8_t data)
{
    ds1302_io_output();

    for (uint8_t bit = 0U; bit < 8U; bit++)
    {
        R_BSP_PinWrite(DS1302_IO_PIN,
                       (0U != (data & 0x01U)) ? BSP_IO_LEVEL_HIGH : BSP_IO_LEVEL_LOW);
        ds1302_delay();
        R_BSP_PinWrite(DS1302_SCLK_PIN, BSP_IO_LEVEL_HIGH);
        ds1302_delay();
        R_BSP_PinWrite(DS1302_SCLK_PIN, BSP_IO_LEVEL_LOW);
        ds1302_delay();
        data >>= 1U;
    }
}

static uint8_t ds1302_read_byte(void)
{
    uint8_t data = 0U;

    for (uint8_t bit = 0U; bit < 8U; bit++)
    {
        if (0U != R_BSP_PinRead(DS1302_IO_PIN))
        {
            data |= (uint8_t) (1U << bit);
        }

        R_BSP_PinWrite(DS1302_SCLK_PIN, BSP_IO_LEVEL_HIGH);
        ds1302_delay();
        R_BSP_PinWrite(DS1302_SCLK_PIN, BSP_IO_LEVEL_LOW);
        ds1302_delay();
    }

    return data;
}

static void ds1302_write_register(uint8_t address, uint8_t data)
{
    R_BSP_PinWrite(DS1302_CE_PIN, BSP_IO_LEVEL_LOW);
    R_BSP_PinWrite(DS1302_SCLK_PIN, BSP_IO_LEVEL_LOW);
    ds1302_delay();
    R_BSP_PinWrite(DS1302_CE_PIN, BSP_IO_LEVEL_HIGH);
    ds1302_delay();

    ds1302_write_byte(address);
    ds1302_write_byte(data);

    R_BSP_PinWrite(DS1302_CE_PIN, BSP_IO_LEVEL_LOW);
    R_BSP_PinWrite(DS1302_SCLK_PIN, BSP_IO_LEVEL_LOW);
    ds1302_delay();
}

static uint8_t ds1302_bcd_to_decimal(uint8_t bcd)
{
    return (uint8_t) (((bcd >> 4U) * 10U) + (bcd & 0x0FU));
}

static uint8_t ds1302_decimal_to_bcd(uint8_t value)
{
    return (uint8_t) (((value / 10U) << 4U) | (value % 10U));
}

static uint8_t ds1302_decode_hours(uint8_t value)
{
    uint8_t hours;

    if (0U != (value & 0x80U))
    {
        hours = ds1302_bcd_to_decimal(value & 0x1FU);
        if (12U == hours)
        {
            hours = 0U;
        }
        if (0U != (value & 0x20U))
        {
            hours = (uint8_t) (hours + 12U);
        }
    }
    else
    {
        hours = ds1302_bcd_to_decimal(value & 0x3FU);
    }

    return hours;
}

void ds1302_init(void)
{
    R_BSP_PinAccessEnable();
    R_BSP_PinWrite(DS1302_CE_PIN, BSP_IO_LEVEL_LOW);
    R_BSP_PinWrite(DS1302_SCLK_PIN, BSP_IO_LEVEL_LOW);
    ds1302_io_input();
    R_BSP_PinAccessDisable();
}

void ds1302_read_time(ds1302_time_t *time)
{
    uint8_t raw[8];

    if (NULL == time)
    {
        return;
    }

    R_BSP_PinAccessEnable();
    R_BSP_PinWrite(DS1302_CE_PIN, BSP_IO_LEVEL_LOW);
    R_BSP_PinWrite(DS1302_SCLK_PIN, BSP_IO_LEVEL_LOW);
    ds1302_delay();
    R_BSP_PinWrite(DS1302_CE_PIN, BSP_IO_LEVEL_HIGH);
    ds1302_delay();

    ds1302_write_byte(DS1302_BURST_READ);
    ds1302_io_input();
    for (uint8_t i = 0U; i < 8U; i++)
    {
        raw[i] = ds1302_read_byte();
    }

    R_BSP_PinWrite(DS1302_CE_PIN, BSP_IO_LEVEL_LOW);
    R_BSP_PinWrite(DS1302_SCLK_PIN, BSP_IO_LEVEL_LOW);
    ds1302_io_input();
    R_BSP_PinAccessDisable();

    time->seconds = ds1302_bcd_to_decimal(raw[0] & 0x7FU);
    time->minutes = ds1302_bcd_to_decimal(raw[1] & 0x7FU);
    time->hours   = ds1302_decode_hours(raw[2]);
    time->date    = ds1302_bcd_to_decimal(raw[3] & 0x3FU);
    time->month   = ds1302_bcd_to_decimal(raw[4] & 0x1FU);
    time->weekday = ds1302_bcd_to_decimal(raw[5] & 0x07U);
    time->year    = ds1302_bcd_to_decimal(raw[6]);
}

void ds1302_set_time(const ds1302_time_t *time)
{
    if (NULL == time)
    {
        return;
    }

    R_BSP_PinAccessEnable();
    ds1302_io_output();
    ds1302_write_register(DS1302_WRITE_PROTECT, 0x00U);
    ds1302_write_register(DS1302_SECONDS, ds1302_decimal_to_bcd(time->seconds % 60U));
    ds1302_write_register(DS1302_MINUTES, ds1302_decimal_to_bcd(time->minutes % 60U));
    ds1302_write_register(DS1302_HOURS, ds1302_decimal_to_bcd(time->hours % 24U));
    ds1302_write_register(DS1302_DATE, ds1302_decimal_to_bcd(time->date));
    ds1302_write_register(DS1302_MONTH, ds1302_decimal_to_bcd(time->month));
    ds1302_write_register(DS1302_WEEKDAY, ds1302_decimal_to_bcd(time->weekday));
    ds1302_write_register(DS1302_YEAR, ds1302_decimal_to_bcd(time->year % 100U));
    ds1302_write_register(DS1302_WRITE_PROTECT, 0x80U);
    ds1302_io_input();
    R_BSP_PinAccessDisable();
}
