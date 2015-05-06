//***************************************************************************
//
//  File Name :		FastIO.h
//
//  Project :		FastIO library for the Atmel 8 bit AVR MCU
//
//  Purpose :		Fast access to the general purpose IO ports on
//					Atmel 8 bit AVR MCUs
//
// The MIT License (MIT)
//
// Copyright (c) 2013-2015 Andy Burgess
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//  Revisions :
//
//      see rcs below
//
//***************************************************************************


#ifndef FASTIO_H_
#define FASTIO_H_


#include "FastIOPorts.h"

#include <avr/io.h>
#include <util/atomic.h>


typedef volatile uint8_t* port_t;
const uint8_t NO_PIN = 255;


// Some helper macros
#define _readBit(value, bit) (((value) >> (bit)) & 0x01)
#define _setBit(value, bit) ((value) |= _BV(bit))
#define _clearBit(value, bit) ((value) &= ~_BV(bit))
#define _writeBit(value, bit, bitvalue) (bitvalue ? _setBit(value, bit) : _clearBit(value, bit))


//! \brief A class describing a single input pint
//! \details Declare a object as follows:
//! InputPin<5> button;
template <uint8_t pin>
class InputPin 
{
public:
	//! \brief Initialises a new instance of the InputPin class
	//! \details The InputPin class is for use with a single input pin
	//! and can optionally set pullups
	//! \param pullup Set true to enable the pullup resistor
	inline InputPin(bool pullup=true) __attribute__ ((always_inline))
	{
		_clearBit(*port_t(_pins<pin>::ddrx), _pins<pin>::bit);			// set as input
		_writeBit(*port_t(_pins<pin>::portx), _pins<pin>::bit, pullup);	// set pullups
	}
	
	//! \brief Gets the state of the input pin
	//! \returns True if the pin is high; otherwise false
	inline bool read() __attribute__ ((always_inline))
	{
		return _readBit(*port_t(_pins<pin>::pinx), _pins<pin>::bit);
	}
	
	//! \brief Gets the state of the input pin
	//! \details This operator overload performs the same as the read method
	inline operator bool() __attribute__ ((always_inline)) 
	{
		return read();
	}
};


//! \brief A class representing a single output pin
//! \details Declare a object as follows:
//! OutputPin<13> led;
template <uint8_t pin>
class OutputPin 
{
	// An digital output with direct port I/O
public:
	//! \brief Initialises a new instance of the OutputPin object
	//! \details Initialises a new output pin by setting
	//! the data direction pin and writing the initial value
	inline OutputPin(bool initValue=LOW) __attribute__ ((always_inline)) 
	{
		_setBit(*port_t(_pins<pin>::ddrx), _pins<pin>::bit);
		write(initValue);
	}
	

	//! \brief Writes the boolean value to the output pin
	//! \param value The value that should be written to the pin
	inline void write(bool value) __attribute__ ((always_inline)) 
	{
		_writeBit(*port_t(_pins<pin>::portx), _pins<pin>::bit, value);
	}
	

	//! \brief Sets the output pin - makes it high	
	inline void set() __attribute__ ((always_inline)) 
	{
		_setBit(*port_t(_pins<pin>::portx), _pins<pin>::bit);
	}


	//! \brief Clears the output pin - makes it low
	inline void clear() __attribute__ ((always_inline)) 
	{
		_clearBit(*port_t(_pins<pin>::portx), _pins<pin>::bit);
	}	
	
	
	//! \brief An operator overload that performs the same as the write method
	inline OutputPin& operator =(bool value)  __attribute__ ((always_inline)) 
	{
		write(value);
		return *this;
	}
	
	
	//! \brief Toggle the output pin
	//! \details Uses the PINx register to toggle the pin
	inline void toggle() __attribute__ ((always_inline)) 
	{
		_setBit(*port_t(_pins<pin>::pinx), _pins<pin>::bit);
	}
	

	//! \brief Reads the current value of the output pin
	//! \returns The current value of the output pin, true if high; false if low
	inline bool read() __attribute__ ((always_inline)) 
	{
		return _readBit(*port_t(_pins<pin>::pinx), _pins<pin>::bit);
	}
	

	//! \brief Operator overload that performs the same as the read method	
	inline operator bool()  __attribute__ ((always_inline)) 
	{
		return read();
	}
};


//! \brief Special case of the OutputPin class that is used for an
//! optional pin that is not used
//! \details A lot of the Arduino libs expect optional pins
//! that are not used
//! \param NO_PIN A constant representing no output pin
template <>
class OutputPin<NO_PIN> 
{
public:
	//! \brief Inialises a new instance of the OutputPin class
	//! \param initValue The initial value of the port pin
	inline OutputPin(bool initValue=LOW) __attribute__ ((always_inline)) {}

	//! \brief Writes a value to the output pin
	//! \param value The value to write		
	inline void write(bool value) __attribute__ ((always_inline)) {}
	
	//! \brief Sets the output pin - makes it high
	inline void set() __attribute__ ((always_inline)) {}


	//! \brief Clears the output pin - makes it low
	inline void clear() __attribute__ ((always_inline)) {}
	
	
	//! \brief This operator overload performs the same as the write method
	inline OutputPin& operator =(bool value) __attribute__ ((always_inline))
	{
		return *this;
	}

	//! \brief Toggles the state of the output pin	
	inline void toggle() __attribute__ ((always_inline)) {}

	//! \brief Returns the state of the output pin
	//! \returns The state of the output pin		
	inline bool read() __attribute__ ((always_inline)) 
	{
		return LOW;
	}
	
	//! \brief An operator overload that performs the same as the read method
	inline operator bool() __attribute__ ((always_inline))
	{
		return read();
	}
};


//! \brief A class to represent one of more bits in an eight bit input port
//! \brief The startBit and numBit parameters specify the starting order and
//! number of bits.
//! \note The bits are contiguous in the port
template <class port, uint8_t startBit=0, uint8_t numBits=8>
class InputPort
{
public:
	//! \brief Initialises a new instance of the InputPort class
	inline InputPort(bool pullup=true)  __attribute__ ((always_inline)) 
	{
		*port_t(port::ddrx) &= (~mask);			// set pins as input
		*port_t(port::portx) &= (~mask);		// clear pullups
		if (pullup)
			*port_t(port::portx) |= mask;		// set pullups as necessary
	}

	//! \brief Reads the bits of the input port
	//! \details The input port is read and a mask is applied before
	//! shifting the bits right
	//! \returns An uint8_t value representing the bits
	inline uint8_t read() __attribute__ ((always_inline))
	{
		return (*port_t(port::pinx) & mask) >> startBit;
	}
	

	//! \brief An operator overload that returns the input port
	//! \details Performs the same as the read method
	inline operator uint8_t() __attribute__ ((always_inline))
	{
		return read();
	}

private:
	static const uint8_t mask = ((uint8_t(1) << numBits) - 1) << startBit;
};


//! \brief A class to represent one of more bits of an eight bit output port
//! \brief The startBit and numBit parameters specify the starting order and
//! number of bits.
//! \note The bits are contiguous in the port
template <class port, uint8_t startBit=0, uint8_t numBits=8>
class OutputPort 
{
public:
	//! \brief Initialises a new instance of the OutputPort class
	inline OutputPort() __attribute__ ((always_inline))
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
		{
			// set port pin directions to output
			*port_t(port::ddrx) |= mask;
		}
	}

	//! \brief Write the supplied value to the output port
	inline void write(uint8_t value) __attribute__ ((always_inline))
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) 
		{
			uint8_t v = *port_t(port::pinx);
			uint8_t shifted = value << startBit;
			v |= shifted & mask;
			v &= (shifted | ~mask);
			*port_t(port::portx) = v;
		}
	}
	

	//! \brief An operator overload that performs the same as the write method	
	inline OutputPort& operator =(uint8_t value) __attribute__ ((always_inline))
	{
		write(value);
		return *this;
	}


	//! \brief Returns the current value of the output port
	inline uint8_t read() __attribute__ ((always_inline))
	{
		return (*port_t(port::pinx) & mask) >> startBit;
	}


	//! \brief An operator overload that performs the same as the read method
	inline operator uint8_t() __attribute__ ((always_inline))
	{
		return read();
	}

private:
	static const uint8_t mask = ((uint8_t(1) << numBits) - 1) << startBit;
};


//! \brief A special case for the output port where all 8 pins are used
template <class port>
class OutputPort<port, 0, 8> 
{
public:
	//! \brief Initialises a new instance of the OutputPort object
	inline OutputPort() __attribute__ ((always_inline))
	{
		// set port pin directions to output
		*port_t(port::ddrx) = 0xFF;
	}

	//! \brief Sets the output pins to the value supplied
	//! \param value The value to write to the port pins
	inline void write(uint8_t value) __attribute__ ((always_inline))
	{
		*port_t(port::portx) = value;
	}

	//! \brief An operator overload than performs the same as the write method
	inline OutputPort& operator =(uint8_t value) __attribute__ ((always_inline))
	{
		write(value);
		return *this;
	}

	//! \brief Returns the value of the data port
	//! \returns The value of the data port
	inline uint8_t read() __attribute__ ((always_inline))
	{
		return *port_t(port::pinx);
	}

	//! \brief An operator overload that performs the same as the read method
	inline operator uint8_t() __attribute__ ((always_inline))
	{
		return read();
	}
};


#endif /* FASTIO_H_ */
