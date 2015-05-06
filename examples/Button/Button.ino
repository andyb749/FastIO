// Example sketch for FastIO
// This sketch has absolutely no dependencies of the Arduino library
// with the exception of the main function that calls setup() and loop()


#include "FastIO.h"


// define a output pin - use the one with the LED
OutputPin<LED_BUILTIN> led;

// define an input pin - by default the pullups are enabled
// to disable pullups, use InputPin<2> button(false);
InputPin<2> button();


void setup()
{
}


void loop()
{
	// check the state of the button
	if (button)
	{
		// if the button is active, then set the LED active
		led = true;
	}
	else
	{
		// otherwise, switch off the LED
		led = false;
	}
}