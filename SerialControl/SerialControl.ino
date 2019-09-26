#include "SerialControlPorts.hpp"
#include "SerialControlMessages.hpp"

#define SPACE 32
#define COLON 58
#define ZERO 48
#define NINE 57

AnalogOutPorts ports = AnalogOutPorts();
unsigned char incomingBytes[62];
boolean serialDebugging = false;

byte *readSerial();
void eatSerial();
void printIncomingSerial(byte numBytes, unsigned char *incomingBytes);
boolean interpretSerial(byte numBytes, unsigned char *incomingBytes);
unsigned char readByte(unsigned char *incomingBytes, byte start, byte length);
void debugAllSerial(unsigned char *allSerial, byte length);

void setup() {
  Serial.begin(9600);

  ports.begin();

  messages::greeting();
}

void loop() {
  // read the next serial message
  delay(30); // give the serial message ample time to be written
  byte numBytes = readSerial(incomingBytes);
  ports.updateVals();
  
  if (numBytes != 0)
  {
    if (serialDebugging)
    {
      printIncomingSerial(numBytes, incomingBytes);
    }
    interpretSerial(numBytes, incomingBytes);
  }
}

/**
 * Interprets the next command from the serial console, as read by readSerial(...).
 * 
 * @return true if state has changed, false for no state change
 */
boolean interpretSerial(byte numBytes, unsigned char *incomingBytes)
{
  unsigned char i, pin, value, command, subCommand, pinIndex;
  boolean found;
  
  command = incomingBytes[0];
  switch (command)
  {
    case 'A': // toggle serial debugging
      serialDebugging = !serialDebugging;
      if (serialDebugging)
      {
        Serial.println("Serial debugging turned on");
      }
      break;
    case 'B': // display available ports
      messages::analogOutPrefix();
      ports.writeAnalogOutPorts();
      Serial.println("");
      break;
    case 'C': // analog write
      subCommand = incomingBytes[1];

      // get the pin index
      switch (subCommand) // 1
      {
        case 'a':
        case 'b':
          pin = readByte(incomingBytes, 2, 2);
          pinIndex = ports.analogOutLookup(pin);
          if (pinIndex < 0)
          {
            messages::unknownAnalogPin(pin);
          }
          break;
        default:
          messages::printUnrecognizedSubCommand(command, subCommand);
          messages::errorMessage();
          return false;
      } // switch (subCommand) 'C'.1
      if (pinIndex < 0)
      {
        break;
      }

      // perform appropriate command for the pin index
      switch (subCommand) // 2
      {
        case 'a':
          Serial.println(ports.getVal(pinIndex));
          Serial.write(ACK);
          break;
        case 'b':
          value = readByte(incomingBytes, 4, 3);
          ports.writeVal(pinIndex, value);
          Serial.write(ACK);
          break;
      } // switch (subCommand) 'C'.2
      
      Serial.println("");
      return true;
    case '?':
      subCommand = incomingBytes[1];
      switch (subCommand)
      {
        case 'A':
          messages::helpSerialDebugToggle();
          break;
        case 'B':
          messages::helpPinout();
          break;
        case 'C':
          messages::helpAnalogWrite(ports);
          break;
        default:
          messages::printUnrecognizedSubCommand(command, subCommand);
          messages::errorMessage();
          return false;
      } // switch (subCommand) '?'
      break;
    default:
      messages::printUnrecognizedCommand(command);
      messages::errorMessage();
      return false;
  } // switch (command)
  
  Serial.write(READY);
  Serial.println("");
  return false;
}

/**
 * Reads the byte value from the given chars array, from the start position to 
 * (start + length - 1) position.
 */
unsigned char readByte(unsigned char *chars, byte start, byte length)
{
  byte retval = 0;
  byte end = start + length;
  for (byte i = start; i < end; i++)
  {
    retval = retval * 10 + (chars[i] - ZERO);
  }
  return retval;
}

/**
 * Reads the next message from the serial console.
 * 
 * Expected format is:
 * a:b
 * Where "a" is the number of bytes to be read, and "b" is those bytes.
 * 
 * @return "a" the number of bytes actually read
 */
byte readSerial(unsigned char *incomingBytes)
{
  byte allSerial[64];
  int numBytes;
  byte next, nextChar, exactLocation;
  
  // check for serial data
  if (!Serial.available())
  {
    return NULL;
  }

  // how many bytes to read?
  numBytes = 0;
  exactLocation = 0;
  for (nextChar = 0; Serial.available(); delay(30))
  {
    nextChar = Serial.read();
    allSerial[exactLocation++] = nextChar;
    if (nextChar == COLON)
    {
      break;
    }
    if (nextChar < ZERO || nextChar > NINE)
    {
      debugAllSerial(allSerial, exactLocation);
      eatSerial();
      messages::errorMessage();
      return 0;
    }
    numBytes = numBytes * 10 + (nextChar - ZERO);
  } // for (byte nextChar = 0; Serial.available(); delay(30))
  delay(numBytes * 10); // let the serial input finish reading
  if (serialDebugging)
  {
    Serial.write("num expected bytes: ");
    Serial.println(numBytes);
  }
  
  // read up to the next numBytes bytes
  for (next = 0; Serial.available() && next < numBytes; next++)
  {
    incomingBytes[next] = Serial.read();
    allSerial[exactLocation++] = incomingBytes[next];

    // check for an abort signal
    if (incomingBytes[next] == SPACE)
    {
      debugAllSerial(allSerial, exactLocation);
      eatSerial();
      Serial.write(READY);
      return next;
    }
  } // for (byte next = 0; Serial.available(); next++)

  debugAllSerial(allSerial, exactLocation);
  if (serialDebugging)
  {
    Serial.write("num read bytes: ");
    Serial.println(next);
  }  
  return (next);
}

/**
 * If serial debugging is true, then prints the characters given by allSerial up to length.
 */
void debugAllSerial(unsigned char *allSerial, byte length)
{
  if (serialDebugging)
  {
    Serial.write("in -> ");
    for (byte i = 0; i < length; i++)
    {
      Serial.write(allSerial[i]);
    }
    Serial.println("");
  }
}

/**
 * Clears the serial buffer.
 */
void eatSerial()
{
  while (Serial.available())
  {
    Serial.read();
  }
}

/**
 * Prints the latest value read from readSerial(...) to the serial console, usually for deubbing purposes.
 */
void printIncomingSerial(byte numBytes, unsigned char *incomingBytes)
{
  Serial.print(numBytes);
  Serial.write(" - ");
  for (byte i = 0; i < numBytes; i++)
  {
    if (i % 4 == 0)
    {
      if (i > 0)
      {
        Serial.write(" ");
      }
      Serial.print(i / 4);
      Serial.write(":");
    }
    Serial.write(incomingBytes[i]);
  }
  Serial.println("");
}

