#include "SerialControlMessages.hpp"

namespace messages
{
	void greeting()
	{
		Serial.write("Use \"?\" to get a list of available serial commands.");newLine();
		Serial.write(READY);newLine();
	}

	void analogOutPrefix()
	{
		Serial.write("Analog out: ");
	}

	void newLine()
	{
		Serial.println("");
	}

	void helpSerialDebugToggle()
	{
		Serial.write("A: Useful in helping to understand why serial commands are failing.\n");
	}

	void helpPinout()
	{
		Serial.write("B: Provides a list of port numbers for reading or writing values to.\n");
	}

	void helpAnalogWrite(AnalogOutPorts ports)
	{
		Serial.write("C: Write a value between 0 and 255 to an analog out port.\n");
		Serial.write("   Format: a#{1} or b#{1}#{2}\n");
		Serial.write("   a: ask for the last written value to the port\n");
		Serial.write("   b: write a value to the port\n");
		Serial.write("   #{1}: length 2, any of ports { ");ports.writeAnalogOutPorts();Serial.write(" }\n");
		Serial.write("   #{2}: length 3, value from 000 to 255\n");
		Serial.write("   Example \"7:Cb09050\" to write the value \"50\" to port \"9\"\n");
	}

	void unknownAnalogPin(byte pin)
	{
		Serial.write("Unknown analog out port ");
		Serial.println(pin);
	}

	/**
	 * Print this message to explain to the user how to use the serial console.
	 * 
	 * The last thing it prints is "READY\n".
	 */
	void errorMessage()
	{
		Serial.write("Unrecognized serial input.\nRecognized values are:\n");
		Serial.write("-. \" \" (space): abort current command (and clear serial input buffer)\n");
		Serial.write("-. A toggle serial debugging\n");
		Serial.write("-. B display available ports\n");
		Serial.write("-. C analog write\n");
		Serial.write("-. ? request more information\n");
		Serial.write("-.-. A-C: the command to get more information about\n");
		Serial.write("Start commands with \"#...:\", where\n    \"#...\" is the number of bytes to follow the colon \":\".\n");
		Serial.write("When reading the serial console is finished, will return either ");
		Serial.write(READY);Serial.write(" or ");Serial.write(ACK);newLine();
		newLine();
		Serial.write(READY);newLine();
	}

	/**
	 * Prints "Unrecognized subcommand {subCommand} for command {command}\n" to Serial.
	 */
	void printUnrecognizedSubCommand(unsigned char command, unsigned char subCommand)
	{
		Serial.write("Unrecognized subcommand ");
		Serial.write(subCommand);
		Serial.write(" for command ");
		Serial.write(command);
		newLine();
	}

	void printUnrecognizedCommand(unsigned char command)
	{
		Serial.write("Unrecognized command ");Serial.write(command);newLine();
	}
}
