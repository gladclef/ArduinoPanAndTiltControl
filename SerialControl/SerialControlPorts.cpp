#include "SerialControlPorts.hpp"

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
		pinMode(analogOutPorts[i], OUTPUT);
	}
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

void AnalogOutPorts::writeVal(byte pinIndex, unsigned char value)
{
	analogWriteVals[pinIndex] = value;
	analogWrite(analogOutPorts[pinIndex], value);
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
