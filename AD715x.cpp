/*
AD715x Library
.cpp file

This library is used to simplify the controll scheme of the AD715x chip for use with a bioreactor controller.


Lukas Jaworski

University of Miami
2016
*/

#include <AD715x.h>

AD715x::AD715x(){
	currentReading.Capacitance = 0;
	currentRegister = STATUS;
	regSettings = {0,0,0,0,0,0};
}

uint16_t AD715x::readCapacitance(){
	currentRegister = CH1DATA;
	Wire.beginTransmission(deviceAddress);
	Wire.write(currentRegister);
	Wire.endTransmission(false);
	Wire.requestFrom(deviceAddress, uint8_t(2));
	if (Wire.available()){
		for (int8_t i = 1; i >= 0; i--){
			currentReading.bytes[i] = Wire.read();
		}
	}
	Wire.endTransmission();
	return currentReading.Capacitance;
}

uint8_t AD715x::readRegister(uint8_t address){
	uint8_t registerValue;
	Wire.beginTransmission(deviceAddress);
	Wire.write(address);
	Wire.endTransmission(false);
	Wire.requestFrom(deviceAddress, uint8_t(1));
	registerValue = Wire.read();
	Wire.endTransmission();
	return registerValue;
}

void AD715x::setMode(capConfig configuration){
	currentRegister = CONFIG;
	regSettings.Config = ((regSettings.Config & 0b00011000) | configuration);
	Wire.beginTransmission(deviceAddress);
	Wire.write(currentRegister);
	Wire.write(regSettings.Config);
	Wire.endTransmission();
	return;
}

bool AD715x::enableChannel(uint8_t channel, bool turnOn){
	currentRegister = CONFIG;
	if (turnOn){
		if (channel == 1){
			regSettings.Config = ((regSettings.Config & 0b00001111) | (1<<4));
		}
		else if (channel == 2){
			regSettings.Config = ((regSettings.Config & 0b00010111) | (1<<3));
		}
	}
	else {
		if (channel == 1){
			regSettings.Config = ((regSettings.Config & 0b00001111));
		}
		else if (channel == 2){
			regSettings.Config = ((regSettings.Config & 0b00010111));
		}
	}
	Wire.beginTransmission(deviceAddress);
	Wire.write(currentRegister);
	Wire.write(regSettings.Config);
	Wire.endTransmission();
	return turnOn;
}

bool AD715x::setCAPDAC(CAPDAC select, uint8_t value, bool turnOn){
	uint8_t CAPDAC_settings;
	if (!select){
		currentRegister = CAPDAC_POS;
		if (turnOn){
			regSettings.CAPDAC_Pos = ((regSettings.CAPDAC_Pos & 0b10000000) | (value));
		}
		else {
			regSettings.CAPDAC_Pos = ((regSettings.CAPDAC_Pos & 0b00000000));
		}
		CAPDAC_settings = regSettings.CAPDAC_Pos;
	}
	else {
		currentRegister = CAPDAC_NEG;
		if (turnOn){
			regSettings.CAPDAC_Neg = ((regSettings.CAPDAC_Neg & 0b10000000) | (value));
		}
		else {
			regSettings.CAPDAC_Neg = ((regSettings.CAPDAC_Neg & 0b00000000));
		}
		CAPDAC_settings = regSettings.CAPDAC_Neg;
	}

	Wire.beginTransmission(deviceAddress);
	Wire.write(currentRegister);
	Wire.write(CAPDAC_settings);
	Wire.endTransmission();
	return turnOn;
}

void AD715x::setRange(uint8_t channel, uint8_t range){
	uint8_t Setup_settings;
	if (channel == 1){
		currentRegister = CH1SETUP;
		regSettings.Ch1_Setup = ((regSettings.Ch1_Setup & 0b00100000) | (range<<6));
		Setup_settings = regSettings.Ch1_Setup;
	}
	else {
		currentRegister = CH2SETUP;
		regSettings.Ch2_Setup = ((regSettings.Ch2_Setup & 0b00100000) | (range<<6));
		Setup_settings = regSettings.Ch2_Setup;
	}

	Wire.beginTransmission(deviceAddress);
	Wire.write(currentRegister);
	Wire.write(Setup_settings);
	Wire.endTransmission();
	return;
}

bool AD715x::setCAPDIFF(uint8_t channel, bool turnOn){
	uint8_t Setup_settings;
	if (turnOn){
		if (channel == 1){
			regSettings.Ch1_Setup = ((regSettings.Ch1_Setup & 0b11000000) | (1<<5));
			currentRegister = CH1SETUP;
			Setup_settings = regSettings.Ch1_Setup;
		}
		else if (channel == 2){
			regSettings.Ch1_Setup = ((regSettings.Ch1_Setup & 0b11000000) | (1<<5));
			currentRegister = CH2SETUP;
			Setup_settings = regSettings.Ch1_Setup;
		}
	}
	else {
		if (channel == 1){
			regSettings.Ch1_Setup = ((regSettings.Ch1_Setup & 0b11000000));
			currentRegister = CH1SETUP;
			Setup_settings = regSettings.Ch1_Setup;
		}
		else if (channel == 2){
			regSettings.Ch2_Setup = ((regSettings.Ch2_Setup & 0b11000000));
			currentRegister = CH2SETUP;
			Setup_settings = regSettings.Ch2_Setup;
		}
	}
	Wire.beginTransmission(deviceAddress);
	Wire.write(currentRegister);
	Wire.write(Setup_settings);
	Wire.endTransmission();
	return turnOn;
}

void AD715x::setConversionTime(conversionTime convTime){
	currentRegister = CONFIG2;
	regSettings.Config2 = ((regSettings.Config & 0b00000000) | (convTime<<4));
	Wire.beginTransmission(deviceAddress);
	Wire.write(currentRegister);
	Wire.write(regSettings.Config2);
	Wire.endTransmission();
	return;
}