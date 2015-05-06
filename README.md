# FastIO
Fast access to Atmel 8bit AVR MCU

This library allows very fast access to MCU pins and ports in many cases using a single instruction
to read/write or modify a pin or port.

# Dependencies
There are absolutely no dependencies on the Arduino environment and these files can be used outwith the
Arduino IDE.

# Caveats
To map Arduino type pins to the MCU hardware the file FastIOPins.h has various definitions.   Currently the standard
Arduino Uno, Pro Micro, Mini, Mega and a special that I built myself are supported.   To add support for additional Arduinos look for the #if defined (ARDUINO_AVR_XXXX) section.   This is name that the Arduino environment uses for a board definition.
Add a _defPin(pinNumber, PORT, PIN) combination for each pin.   Also add definitions for SS, MOSI, MISO, SCK SDA, SCL and LED_BUILTIN.

# Using Outwith the Arduino environment
In your makefile or project configuration you will need to have a definition of ARDUINO_AVR_MEGA2560, ARDUINO_AVR_UNO, ARDUINO_AVR_YUN, ARDUINO_AVR_DUEMILANOVE, ARDUINO_AVR_NANO, ARDUINO_AVR_MINI, ARDUINO_AVR_ETHERNET, ARDUINO_AVR_FIO, ARDUINO_AVR_BT, ARDUINO_AVR_LILYPAD, ARDUINO_AVR_PRO, ARDUINO_AVR_NG OR ARDUINO_AVR_SANGUINO.
