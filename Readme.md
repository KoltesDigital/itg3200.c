# ITG3200.c

Simple wrapper for ITG3200 gyroscope on I2C.

Tested on a Raspberry Pi 2.

## Build

Build, and optionally install, with [scons](http://scons.org/).

	scons
	sudo scons install

## Usage

All functions take a file handle as first parameter (obtained with `open` and released with `close`), and return `0` when successful or `-1` on failure.

Example:

	static int file;

	static int closeAndExit(int code)
	{
		close(file);
		return code;
	}

	int main(void)
	{
		file = open("/dev/i2c-1", O_RDWR);
		if (file < 0)
			return -1;

		if (ITG3200_Init(file, ITG3200_ID, true))
			return closeAndExit(-1);

	    struct ITG3200_Acquisition confAcquisition = {
	        .lowPassFilter = ITG3200_LOWPASSFILTER_42,
	        .sampleRateDivider = 0,
	    };
	    if (ITG3200_ConfigureAcquisition(file, &confAcquisition))
			return closeAndExit(-1);

	    struct ITG3200_Power confPower = {
	        .clockSource = ITG3200_CLOCKSOURCE_PLL_X,
	    };
	    if (ITG3200_ConfigurePower(file, &confPower))
			return closeAndExit(-1);

		short x, y, z;
		if (ITG3200_ReadData(file, &x, &y, &z))
			return closeAndExit(-1);

		return closeAndExit(0);
	}


### ITG3200_Init(int file, unsigned char id, bool check)

`id` is the device I2C address. It is usually `ITG3200_ID` (0x68).

If `check` is true, the function reads the device identification registers and checks against the expected values. This inherently checks that the device and the I2C bus are working.

## License

Copyright 2016 Bloutiouf

[MIT License](https://opensource.org/licenses/MIT)
