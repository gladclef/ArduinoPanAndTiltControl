#ifndef SERIAL_CONTROL_PORTS
#define SERIAL_CONTROL_PORTS

#include "Arduino.h"

// modify this and the values in begin() for your model of arduino
#define ANALOG_OUT_LENGTH 6

class AnalogOutPorts
{
private:
	unsigned char analogWriteVals[ANALOG_OUT_LENGTH];
	byte analogOutPorts[ANALOG_OUT_LENGTH];

public:
	void begin();
	void writeAnalogOutPorts();
	unsigned char getVal(byte pinIndex);
	void writeVal(byte pinIndex, unsigned char value);
	byte analogOutLookup(byte pin);
};

#endif
