# ArduinoSerialControl
Very verbose serial control of an Arduino.

## Purpose
This "library" is meant to be used to give basic, human-usable control of an Arduino through the USB serial interface.

## Designed for Uno
I have an Arduino Uno and designed this library for use with it, but you can use it with other versions of Arduino by modifying SerialControlPorts.hpp/cpp.

## How To
Simply download/fork the library and run on your Arduino! Command promts are provided by the program. Type "?" to get started.

## Example Usage
Here I use this library to set the brightness of an LED from the serial console.
![serial console output](/pics/exampleUsage.PNG)

Commands for this example:

1. "?"
2. "1:A"
3. "1:B"
4. "7:Cb09050"
5. "4:Ca09"

![use with arduino](/pics/exampleSetup.jpg)
