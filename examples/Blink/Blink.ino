// Example sketch for FastIO
// This sketch has absolutely no dependencies of the Arduino library
// with the exception of the main function that calls setup() and loop()
//
// The original Arduino blink for the Mega2560 is 1518 program bytes
// with 9 data bytes.
// By comparison this sketch in the Arduino environment is 710 bytes.
// Using led.set and led.clear is still only 716 bytes
// An equivalent c++ program in Atmel Studio is 346 bytes
// Alternatively using DDRB |= _BV(PB7) to set the pin direction
// and PINB |= _BV(PB7) takes 286 bytes.   Most of this is the C++
// runtime environment.


// includes - uses the Atmel provided delay functions
#include <util/delay.h>
#include "FastIO.h"


// Object declarations
OutputPin<LED_BUILTIN> led;


// Setup function
void setup()
{
}


// Loop function
void loop()
{
  led.toggle();
  delay1s();
}


// A 1 second delay without any Arduino libs
void delay1s ()
{
  for (int i = 0; i < 100; i++)
    _delay_ms (10);
}