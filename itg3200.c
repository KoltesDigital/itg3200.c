#include <sys/ioctl.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>

#include "itg3200.h"

#define READ(N) \
	if (read(file, buffer, N) != N) \
		return -1;

#define WRITE(N) \
	if (write(file, buffer, N) != N) \
		return -1;

int ITG3200_Init(int file, unsigned char id, bool check)
{
    if (ioctl(file, I2C_SLAVE, id) < 0)
        return -1;

    if (check)
    {
        unsigned char buffer[1];

        buffer[0] = 0x00;
        WRITE(1);

        READ(1);
        if (buffer[0] != id)
            return -1;
    }

    return 0;
}

int ITG3200_ConfigureAcquisition(int file, const struct ITG3200_Acquisition *conf)
{
    unsigned char buffer[3];

    buffer[0] = 0x15;
    buffer[1] = conf->sampleRateDivider;
    buffer[2] = 0x18 | (conf->lowPassFilter & 0x07);
    WRITE(3);

    return 0;
}

int ITG3200_ConfigureInterrupt(int file, const struct ITG3200_Interrupts *conf)
{
    unsigned char buffer[2];

    buffer[0] = 0x17;
    buffer[1] = (conf->active & 0x80) |
        (conf->drive & 0x40) |
        (conf->latch & 0x20) |
        (conf->clear & 0x10) |
        (conf->enable & 0x05);
    WRITE(2);

    return 0;
}

int ITG3200_ConfigurePower(int file, const struct ITG3200_Power *conf)
{
    unsigned char buffer[2];

    buffer[0] = 0x3E;
    buffer[1] = (conf->sleep ? 0x40 : 0x00) |
        (conf->standby & 0x38) |
        (conf->clockSource & 0x07);
    WRITE(2);

    return 0;
}

int ITG3200_ReadData(int file, short *x, short *y, short *z)
{
    unsigned char buffer[6];

    buffer[0] = 0x1D;
    WRITE(1);

    READ(6);
    *x = ((buffer[2] << 8) | buffer[3]);
    *y = ((buffer[0] << 8) | buffer[1]);
    *z = ((buffer[4] << 8) | buffer[5]);

    return 0;
}

int ITG3200_ReadInterruptSources(int file, enum ITG3200_Interrupt *sources)
{
    unsigned char buffer[1];

    buffer[0] = 0x1A;
    WRITE(1);

    READ(1);
    *sources = buffer[0];

    return 0;
}

int ITG3200_ReadTemperature(int file, short *temperature)
{
    unsigned char buffer[2];

    buffer[0] = 0x1B;
    WRITE(1);

    READ(2);
    *temperature = ((buffer[0] << 8) | buffer[1]);

    return 0;
}

float ITG3200_ConvertTemperature(float rawTemperature)
{
    return 35.0f + (rawTemperature + 13200.0f) / 280.0f;
}
