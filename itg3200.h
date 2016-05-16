// ITG3200 gyroscope
// Copyright 2016 Bloutiouf
// https://opensource.org/licenses/MIT

#ifndef ITG3200_H
#define ITG3200_H

#include <stdbool.h>

#define ITG3200_ID 0x68

enum ITG3200_ClockSource
{
    ITG3200_CLOCKSOURCE_INTERNAL = 0x00,
    ITG3200_CLOCKSOURCE_PLL_X = 0x01,
    ITG3200_CLOCKSOURCE_PLL_Y = 0x02,
    ITG3200_CLOCKSOURCE_PLL_Z = 0x03,
    ITG3200_CLOCKSOURCE_PLL_EXTERNAL_37K768 = 0x04,
    ITG3200_CLOCKSOURCE_PLL_EXTERNAL_19M2 = 0x05,
};

// bitfield
enum ITG3200_Interrupt
{
    ITG3200_INTERRUPT_DATA = 0x01,
    ITG3200_INTERRUPT_PLL = 0x04,
};

enum ITG3200_InterruptActive
{
    ITG3200_INTERRUPTACTIVE_HIGH = 0x00,
    ITG3200_INTERRUPTACTIVE_LOW = 0x80,
};

enum ITG3200_InterruptClear
{
    ITG3200_INTERRUPTCLEAR_ANYREGISTERREAD = 0x00,
    ITG3200_INTERRUPTCLEAR_STATUSREGISTERREAD = 0x10,
};

enum ITG3200_InterruptDrive
{
    ITG3200_INTERRUPTDRIVE_OPENDRAIN = 0x00,
    ITG3200_INTERRUPTDRIVE_PUSHPULL = 0x40,
};

enum ITG3200_InterruptLatch
{
    ITG3200_INTERRUPTLATCH_UNTILCLEAR = 0x00,
    ITG3200_INTERRUPTLATCH_PULSE = 0x20,
};

enum ITG3200_LowPassFilter
{
    ITG3200_LOWPASSFILTER_256 = 0x00,
    ITG3200_LOWPASSFILTER_188 = 0x01,
    ITG3200_LOWPASSFILTER_98 = 0x02,
    ITG3200_LOWPASSFILTER_42 = 0x03,
    ITG3200_LOWPASSFILTER_20 = 0x04,
    ITG3200_LOWPASSFILTER_10 = 0x05,
    ITG3200_LOWPASSFILTER_5 = 0x06,
};

// bitfield
enum ITG3200_Standby
{
    ITG3200_STANDBY_X = 0x20,
    ITG3200_STANDBY_Y = 0x10,
    ITG3200_STANDBY_Z = 0x08,
};

struct ITG3200_Acquisition
{
    enum ITG3200_LowPassFilter lowPassFilter;
    char sampleRateDivider;
};

struct ITG3200_Interrupts
{
    enum ITG3200_InterruptActive active;
    enum ITG3200_InterruptClear clear;
    enum ITG3200_InterruptDrive drive;
    enum ITG3200_Interrupt enable;
    enum ITG3200_InterruptLatch latch;
};

struct ITG3200_Power
{
    enum ITG3200_ClockSource clockSource;
    bool sleep;
    enum ITG3200_Standby standby;
};

int ITG3200_Init(int file, unsigned char id, bool check);

int ITG3200_ConfigureAcquisition(int file, const struct ITG3200_Acquisition *conf);
int ITG3200_ConfigureInterrupt(int file, const struct ITG3200_Interrupts *conf);
int ITG3200_ConfigurePower(int file, const struct ITG3200_Power *conf);

int ITG3200_ReadData(int file, short *x, short *y, short *z);
int ITG3200_ReadInterruptSources(int file, enum ITG3200_Interrupt *sources);
int ITG3200_ReadTemperature(int file, short *temperature);

// Degree Celsius
float ITG3200_ConvertTemperature(float rawTemperature);

#endif
