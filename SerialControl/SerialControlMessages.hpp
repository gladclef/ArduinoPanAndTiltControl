#ifndef SERIAL_CONTROL_MESSAGES
#define SERIAL_CONTROL_MESSAGES

#include "Arduino.h"
#include "SerialControlPorts.hpp"

#define READY "READY"
#define ACK "ACK"

namespace messages
{
	void greeting();
	void analogOutPrefix();;
	void newLine();
	void helpSerialDebugToggle();
	void helpPinout();
	void helpAnalogWrite(AnalogOutPorts ports);
	void unknownAnalogPin(byte pin);
	void errorMessage();
	void printUnrecognizedSubCommand(unsigned char command, unsigned char subCommand);
	void printUnrecognizedCommand(unsigned char command);
}

#endif
