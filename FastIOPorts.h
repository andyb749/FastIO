//***************************************************************************
//
//  File Name :		FastIOPorts.h
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


#ifndef FASTIOPORTS_H_
#define FASTIOPORTS_H_

#ifndef FASTIO_H_
#error "Do not include FastIOPorts.h, use FastIO.h"
#endif

// We need to undef these because we want to redefine _SFR_ASM_COMPAT macro
// and include avr/io.h again to get numbers from the sfr registers
#undef _AVR_COMMON_H
#undef _AVR_IO_H_
#undef _AVR_IOXXX_H_
#undef _AVR_SFR_DEFS_H_
#undef _SFR_ASM_COMPAT

#define _SFR_ASM_COMPAT 1
#include <avr/io.h>


//! \brief Macro to define a structure for a particular port
//! \param NAME The final name of the structure
//! \param PINREG The PIN SFR of this port
//! \param DATAREG The PORT SFR of this port
//! \param DDRREG The DDR SFR of this port
#define _defPort(NAME, PINREG, DATAREG, DDRREG) \
struct NAME { \
	static const uint16_t pinx = PINREG; \
	static const uint16_t portx = DATAREG; \
	static const uint16_t ddrx = DDRREG; \
}


// Define a structure for each IO port that physically exists on a processor

#ifdef PINA
_defPort(PORT_A, PINA, PORTA, DDRA);
#endif

#ifdef PINB
_defPort(PORT_B, PINB, PORTB, DDRB);
#endif

#ifdef PINC
_defPort(PORT_C, PINC, PORTC, DDRC);
#endif

#ifdef PIND
_defPort(PORT_D, PIND, PORTD, DDRD);
#endif

#ifdef PINE
_defPort(PORT_E, PINE, PORTE, DDRE);
#endif

#ifdef PINF
_defPort(PORT_F, PINF, PORTF, DDRF);
#endif

#ifdef PING
_defPort(PORT_G, PING, PORTG, DDRG);
#endif

#ifdef PINH
_defPort(PORT_H, PINH, PORTH, DDRH);
#endif

#ifdef PINJ
_defPort(PORT_J, PINJ, PORTJ, DDRJ);
#endif

#ifdef PINK
_defPort(PORT_K, PINK, PORTK, DDRK);
#endif

#ifdef PINL
_defPort(PORT_L, PINL, PORTL, DDRL);
#endif


// cleanup - we've finished with the SFR definitions for now
#undef _AVR_COMMON_H
#undef _AVR_IO_H_
#undef _AVR_IOXXX_H_
#undef _AVR_SFR_DEFS_H_
#undef _SFR_ASM_COMPAT


//! \brief A template structure to describe a pin
//! \details Initially an empty structure, see below for use
template <uint8_t pin> struct _pins {};


//! \brief A macro to describe a single IO pin Arduino style
//! \details When used, will create a templated struct that will store
//! all required data to perform operations on a IO port or pin.
//! \param PIN the Arduino pin number
//! \param PORT one of the ports PORT_A to PORT_L defined above
//! \param BIT the bit of the IO port
#define _defPin(PIN, PORT, BIT) \
template <> struct _pins<PIN> : public PORT { \
	static const uint8_t bit = BIT; \
}


// Handle the ARDUINO environment
// This will ensure that the defines of HIGH/LOW/INPUT/OUPUT etc are
// all included
#ifdef ARDUINO

	#if ARDUINO >=100
	 #include "Arduino.h"
	#else
	 #include "WProgram.h"
	#endif

#else

	// For non Arduino environments we need these defined
	#ifndef LOW
	 #define LOW 0
	#endif
	#ifndef HIGH
	 #define HIGH 1
	#endif

#endif	// ARDUINO


// The ARDUINO environment will define one of these from the boards.txt file
// If your board is not defined then it can be added here
#if defined(ARDUINO_AVR_MEGA2560)

// not sure if these would be best as #defines or static const uint8_t
#ifndef ARDUINO

static const uint8_t SS   = 53;
static const uint8_t MOSI = 51;
static const uint8_t MISO = 50;
static const uint8_t SCK  = 52;

static const uint8_t SDA = 20;
static const uint8_t SCL = 21;
static const uint8_t LED_BUILTIN = 13;

static const uint8_t A0 = 54;
static const uint8_t A1 = 55;
static const uint8_t A2 = 56;
static const uint8_t A3 = 57;
static const uint8_t A4 = 58;
static const uint8_t A5 = 59;
static const uint8_t A6 = 60;
static const uint8_t A7 = 61;
static const uint8_t A8 = 62;
static const uint8_t A9 = 63;
static const uint8_t A10 = 64;
static const uint8_t A11 = 65;
static const uint8_t A12 = 66;
static const uint8_t A13 = 67;
static const uint8_t A14 = 68;
static const uint8_t A15 = 69;

#endif	// ARDUINO

// Manually define the pins
_defPin(0, PORT_E, 0);
_defPin(1, PORT_E, 1);
_defPin(2, PORT_E, 4);
_defPin(3, PORT_E, 5);
_defPin(4, PORT_G, 5);
_defPin(5, PORT_E, 3);
_defPin(6, PORT_H, 3);
_defPin(7, PORT_H, 4);
_defPin(8, PORT_H, 5);
_defPin(9, PORT_H, 6);
_defPin(10, PORT_B, 4);
_defPin(11, PORT_B, 5);
_defPin(12, PORT_B, 6);
_defPin(13, PORT_B, 7);
_defPin(14, PORT_J, 1);
_defPin(15, PORT_J, 0);
_defPin(16, PORT_H, 1);
_defPin(17, PORT_H, 0);
_defPin(18, PORT_D, 3);
_defPin(19, PORT_D, 2);
_defPin(20, PORT_D, 1);
_defPin(21, PORT_D, 0);
_defPin(22, PORT_A, 0);
_defPin(23, PORT_A, 1);
_defPin(24, PORT_A, 2);
_defPin(25, PORT_A, 3);
_defPin(26, PORT_A, 4);
_defPin(27, PORT_A, 5);
_defPin(28, PORT_A, 6);
_defPin(29, PORT_A, 7);
_defPin(30, PORT_C, 7);
_defPin(31, PORT_C, 6);
_defPin(32, PORT_C, 5);
_defPin(33, PORT_C, 4);
_defPin(34, PORT_C, 3);
_defPin(35, PORT_C, 2);
_defPin(36, PORT_C, 1);
_defPin(37, PORT_C, 0);
_defPin(38, PORT_D, 7);
_defPin(39, PORT_G, 2);
_defPin(40, PORT_G, 1);
_defPin(41, PORT_G, 0);
_defPin(42, PORT_L, 7);
_defPin(43, PORT_L, 6);
_defPin(44, PORT_L, 5);
_defPin(45, PORT_L, 4);
_defPin(46, PORT_L, 3);
_defPin(47, PORT_L, 2);
_defPin(48, PORT_L, 1);
_defPin(49, PORT_L, 0);
_defPin(50, PORT_B, 3);
_defPin(51, PORT_B, 2);
_defPin(52, PORT_B, 1);
_defPin(53, PORT_B, 0);
_defPin(54, PORT_F, 0);
_defPin(55, PORT_F, 1);
_defPin(56, PORT_F, 2);
_defPin(57, PORT_F, 3);
_defPin(58, PORT_F, 4);
_defPin(59, PORT_F, 5);
_defPin(60, PORT_F, 6);
_defPin(61, PORT_F, 7);
_defPin(62, PORT_K, 0);
_defPin(63, PORT_K, 1);
_defPin(64, PORT_K, 2);
_defPin(65, PORT_K, 3);
_defPin(66, PORT_K, 4);
_defPin(67, PORT_K, 5);
_defPin(68, PORT_K, 6);
_defPin(69, PORT_K, 7);

#elif defined(ARDUINO_AVR_UNO) || \
	defined(ARDUINO_AVR_YUN) || \
	defined(ARDUINO_AVR_DUEMILANOVE) || \
	defined(ARDUINO_AVR_NANO) || \
	defined(ARDUINO_AVR_MINI) || \
	defined(ARDUINO_AVR_ETHERNET) || \
	defined(ARDUINO_AVR_FIO) || \
	defined(ARDUINO_AVR_BT) || \
	defined(ARDUINO_AVR_LILYPAD) || \
	defined(ARDUINO_AVR_PRO) || \
	defined(ARDUINO_AVR_NG)

#ifndef ARDUINO

static const uint8_t SS   = 10;
static const uint8_t MOSI = 11;
static const uint8_t MISO = 12;
static const uint8_t SCK  = 13;

static const uint8_t SDA = 18;
static const uint8_t SCL = 19;
#define LED_BUILTIN 13

#endif	// ARDUINO

_defPin(0, PORT_D, 0);
_defPin(1, PORT_D, 1);
_defPin(2, PORT_D, 2);
_defPin(3, PORT_D, 3);
_defPin(4, PORT_D, 4);
_defPin(5, PORT_D, 5);
_defPin(6, PORT_D, 6);
_defPin(7, PORT_D, 7);
_defPin(8, PORT_B, 0);
_defPin(9, PORT_B, 1);
_defPin(10, PORT_B, 2);
_defPin(11, PORT_B, 3);
_defPin(12, PORT_B, 4);
_defPin(13, PORT_B, 5);
_defPin(14, PORT_C, 0);
_defPin(15, PORT_C, 1);
_defPin(16, PORT_C, 2);
_defPin(17, PORT_C, 3);
_defPin(18, PORT_C, 4);
_defPin(19, PORT_C, 5);

#elif defined(ARDUINO_AVR_SANGUINO)

#ifndef ARDUINO

static const uint8_t SS   = 4;
static const uint8_t MOSI = 5;
static const uint8_t MISO = 6;
static const uint8_t SCK  = 7;

static const uint8_t SDA = 17;
static const uint8_t SCL = 16;
static const uint8_t LED_BUILTIN = 0;

static const uint8_t A0 = 31;
static const uint8_t A1 = 30;
static const uint8_t A2 = 29;
static const uint8_t A3 = 28;
static const uint8_t A4 = 27;
static const uint8_t A5 = 26;
static const uint8_t A6 = 25;
static const uint8_t A7 = 24;

#endif

_defPin(0, PORT_B, 0);
_defPin(1, PORT_B, 1);
_defPin(2, PORT_B, 2);
_defPin(3, PORT_B, 3);
_defPin(4, PORT_B, 4);
_defPin(5, PORT_B, 5);
_defPin(6, PORT_B, 6);
_defPin(7, PORT_B, 7);
_defPin(8, PORT_D, 0);
_defPin(9, PORT_D, 1);
_defPin(10, PORT_D, 2);
_defPin(11, PORT_D, 3);
_defPin(12, PORT_D, 4);
_defPin(13, PORT_D, 5);
_defPin(14, PORT_D, 6);
_defPin(15, PORT_D, 7);
_defPin(16, PORT_C, 0);
_defPin(17, PORT_C, 1);
_defPin(18, PORT_C, 2);
_defPin(19, PORT_C, 3);
_defPin(20, PORT_C, 4);
_defPin(21, PORT_C, 5);
_defPin(22, PORT_C, 6);
_defPin(23, PORT_C, 7);
_defPin(24, PORT_A, 0);
_defPin(25, PORT_A, 1);
_defPin(26, PORT_A, 2);
_defPin(27, PORT_A, 3);
_defPin(28, PORT_A, 4);
_defPin(29, PORT_A, 5);
_defPin(30, PORT_A, 6);
_defPin(31, PORT_A, 7);

#else
	#error "Cannot recognise board type"
#endif


#endif /* FASTIOPORTS_H_ */
