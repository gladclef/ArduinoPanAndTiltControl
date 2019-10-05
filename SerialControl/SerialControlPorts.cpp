#include "SerialControlPorts.hpp"
#include <Servo.h>

#ifndef MIN
#define MIN(a,b) ((a < b) ? a : b)
#define MAX(a,b) ((a > b) ? a : b)
#endif

Servo panServo;
Servo tiltServo;
unsigned char panVal = 90, tiltVal = 20, currPan = 90, currTilt = 20;

void AnalogOutPorts::begin()
{
	analogOutPorts[0] = 3;
	analogOutPorts[1] = 5;
	analogOutPorts[2] = 6;
	analogOutPorts[3] = 9;
	analogOutPorts[4] = 10;
	analogOutPorts[5] = 11;
	for (byte i = 0; i < ANALOG_OUT_LENGTH; i++)
	{
		analogWriteVals[i] = 0;
		//pinMode(analogOutPorts[i], OUTPUT);
	}
	Serial.println("attaching servos");
	panServo.attach(9);
	tiltServo.attach(12);
	Serial.println("attached");
	panServo.write(currPan);
	tiltServo.write(currTilt);
}

/**
 * Writes the analog out ports to Serial. Does not include a newline character.
 */
void AnalogOutPorts::writeAnalogOutPorts()
{
	for (byte i = 0; i < ANALOG_OUT_LENGTH; i++)
	{
		if (i > 0)
		{
			Serial.write(", ");
		}
		Serial.print(analogOutPorts[i]);
	}
}

unsigned char AnalogOutPorts::getVal(byte pinIndex)
{
	return analogWriteVals[pinIndex];
}

writeValPanTilt(byte pin, unsigned char value)
{
	if (pin == 9) {
		uint16_t val = (uint16_t)value * 180 / 255;
//		Serial.print("writing value ");
//		Serial.print(val);
//		Serial.println(" to pan");
		panServo.write(val);
	}
	if (pin == 10) {
		uint16_t val = (uint16_t)value * 180 / 255;
//		Serial.print("writing value ");
//		Serial.print(val);
//		Serial.println(" to tilt");
		tiltServo.write(val);
	}
}

void AnalogOutPorts::updateVals()
{
	if (currPan < panVal) {
		currPan += MIN(2, panVal - currPan);
		writeValPanTilt(9, currPan);
	}
	if (currPan > panVal) {
		currPan -= MIN(2, currPan - panVal);
		writeValPanTilt(9, currPan);
	}
	if (currTilt < tiltVal) {
		currTilt += MIN(1, tiltVal - currTilt);
		writeValPanTilt(10, currTilt);
	}
	if (currTilt > tiltVal) {
		currTilt -= MIN(1, currTilt - tiltVal);
		writeValPanTilt(10, currTilt);
	}
}

void AnalogOutPorts::writeVal(byte pinIndex, unsigned char value)
{
	analogWriteVals[pinIndex] = value;
	//analogWrite(analogOutPorts[pinIndex], value);
	if (analogOutPorts[pinIndex] == 9) {
		panVal = value;
	}
	if (analogOutPorts[pinIndex] == 10) {
		tiltVal = value;
	}
}

/**
 * Find the index in analogOutPorts for the given pin.
 * 
 * @return the index for the given pin, or -1 if the index isn't found
 */
byte AnalogOutPorts::analogOutLookup(byte pin)
{
	for (byte i = 0; i < ANALOG_OUT_LENGTH; i++)
	{
		if (analogOutPorts[i] == pin)
		{
			return i;
		}
	}
	return -1;
}
