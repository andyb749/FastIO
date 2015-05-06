// Example sketch for FastIO
// This sketch has absolutely no dependencies of the Arduino library
// with the exception of the main function that calls setup() and loop()
// This example demonstrates how an entire 8 bit port or part of it can 
// be read in one instruction 



// Atmel delay routines
#include <util/delay.h>

#include "FastIO.h"


// Declaration of our port.  This declaration uses 2 bits of port bits
// starting from bit 6 (bit 6 and bit 7).  If the starting and number of bits
// are omitted, the entire 8 bits will be used.
OutputPort<PORT_B, 6, 2> myPort;


// make a delay of 500ms using the Atmel delay functions
void delay_500ms ()
{
	for (int i=0; i<50; i++)
		_delay_ms (10);
}


void setup()
{
}


void loop()
{
	// loop through all possible values of 2 bits
	for (uint8_t count = 0; count< 4; count++)
	{
		myPort = count;
		delay_500ms();
	}
}
