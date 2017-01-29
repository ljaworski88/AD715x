/*
AD715x Library
.h file

This library is used to simplify the controll scheme of the AD715x chip for use with a bioreactor controller.


Lukas Jaworski

University of Miami
2016
*/

//**NOTE: the function Wire.begin() will need to be called somewhere in the mian body of your program, BEFORE you call any of the library functions
#ifndef AD715x_H
#define AD715x_H
#include <Wire.h>
#include "Arduino.h"

typedef enum capConfig {
	Idle = 0,
	Continuous,
	Single,
	PowerDown,
	OffsetCalibration = 0b101,
	GainCalibration
} capConfig;

typedef enum conversionTime {
	ms5 = 0,
	ms20,
	ms50,
	ms60
} conversionTime;

typedef enum capRegister {
	STATUS 		= 0x00,
	CH1DATA 	= 0x01,
	CH2DATA 	= 0x03,
	CH1OFFSET 	= 0x05,
	CH2OFFSET 	= 0x07,
	CH1GOFFSET 	= 0x09,
	CH2GOFFSET 	= 0x0C,
	CH1SETUP 	= 0x0B,
	CH2SETUP 	= 0x0E,
	CONFIG		= 0x0F,
	CONFIG2		= 0x1A,
	CAPDAC_POS	= 0x11,
	CAPDAC_NEG	= 0x12
	
	
} capRegister;

typedef enum CAPDAC {
	POSITIVE = 0,
	NEGATIVE
} CAPDAC;

typedef struct registerSettings {
	uint8_t Ch1_Setup;
	uint8_t Ch2_Setup;
	uint8_t Config;
	uint8_t Config2;
	uint8_t CAPDAC_Pos;
	uint8_t CAPDAC_Neg;
} registerSettings;

typedef union capReading{
	uint32_t Capacitance;
	uint8_t bytes[4];
} capReading;

class AD715x {
	public:
	AD715x();
	uint16_t readCapacitance();
	uint8_t readRegister(uint8_t address);
	void setMode(capConfig configuration);
	void setRange(uint8_t channel, uint8_t range);
	bool setCAPDIFF(uint8_t channel, bool turnOn = true);
	bool enableChannel(uint8_t channel, bool turnOn = true);
	bool setCAPDAC(CAPDAC select, uint8_t value, bool turnOn = true);
	void setConversionTime(conversionTime convTime);
	
	
	private:
	const uint8_t deviceAddress = 0x48;
	capRegister currentRegister;
	registerSettings regSettings;

	capReading currentReading;
	
	
};
#endif