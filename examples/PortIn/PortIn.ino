// Example sketch for FastIO
// This sketch has absolutely no dependencies of the Arduino library
// with the exception of the main function that calls setup() and loop()
// This example demonstrates how an entire 8 bit port or part of it can 
// be read in one instruction 

// These includes are to allow use of the standard io library functions
// to print messages to the users PC

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "FastIO.h"


// declare our input port - by default all eight bits are used.  To use
// partially declare as follows:
// InputPort<PORT_F, 6, 2> myPort
// This would make PF6 and PF7 our port and return uint8_t values from read()
// between 0 and 3
InputPort<PORT_F> myPort;


// function prototypes
void ioinit();
void uart_init();
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);


void setup()
{
	// setup the UART and stdio library
	ioinit();
}


void loop()
{
	// read the port
	uint8_t inputs = myPort;
	
	// print the value on the user's PC
	printf ("Port = %02X\r", inputs);
}


//=============================================================================
// Everything  below here is infrastructure to allow use of the stdio console
// routines
#define UART_BAUD 57600
#define RX_BUFSIZE 80

void ioinit()
{
	static FILE uart_str;

	// setup UART0
	uart_init();

	// initialise the stream structures
	fdev_setup_stream(&uart_str, uart_putchar, uart_getchar, _FDEV_SETUP_RW);

	// associate stdin/out/err with the uart stream
	stdout = stdin = &uart_str;
}


// initialise the UART to 9600 baud, tx/rx. 8N1
void uart_init()
{
	// set up baudrate
	#if F_CPU < 2000000UL && defined(U2X)
		UCSR0A = _BV(U2X);	// improve baud rate error by using 2x clk
		UBRR0L = (F_CPU / (8UL * UART_BAUD)) - 1;
	#else 
		UBRR0L = (F_CPU / (16UL * UART_BAUD)) - 1;	
	#endif
	
	// enable rx/tx
	UCSR0B = _BV(TXEN0) | _BV(RXEN0);
}


int uart_putchar(char c, FILE *stream)
{
	if (c == '\a')
	{
		fputs("*ring*\n", stderr);
		return 0;
	}
	
	if (c== '\n')
		uart_putchar('\r', stream);
		
	loop_until_bit_is_set (UCSR0A, UDRE0);
	UDR0 = c;
		
	return 0;
}


/*
 * Receive a character from the UART Rx.
 *
 * This features a simple line-editor that allows to delete and
 * re-edit the characters entered, until either CR or NL is entered.
 * Printable characters entered will be echoed using uart_putchar().
 *
 * Editing characters:
 *
 * . \b (BS) or \177 (DEL) delete the previous character
 * . ^u kills the entire input buffer
 * . ^w deletes the previous word
 * . ^r sends a CR, and then reprints the buffer
 * . \t will be replaced by a single space
 *
 * All other control characters will be ignored.
 *
 * The internal line buffer is RX_BUFSIZE (80) characters long, which
 * includes the terminating \n (but no terminating \0).  If the buffer
 * is full (i. e., at RX_BUFSIZE-1 characters in order to keep space for
 * the trailing \n), any further input attempts will send a \a to
 * uart_putchar() (BEL character), although line editing is still
 * allowed.
 *
 * Input errors while talking to the UART will cause an immediate
 * return of -1 (error indication).  Notably, this will be caused by a
 * framing error (e. g. serial line "break" condition), by an input
 * overrun, and by a parity error (if parity was enabled and automatic
 * parity recognition is supported by hardware).
 *
 * Successive calls to uart_getchar() will be satisfied from the
 * internal buffer until that buffer is emptied again.
 */
int uart_getchar(FILE *stream)
{
	uint8_t c;
	char *cp, *cp2;
	static char b[RX_BUFSIZE];
	static char *rxp;

	if (rxp == 0)
	for (cp = b;;)
	{
		loop_until_bit_is_set(UCSR0A, RXC0);

		if (UCSR0A & _BV(FE0))
			return _FDEV_EOF;
		if (UCSR0A & _BV(DOR0))
			return _FDEV_ERR;
		
		c = UDR0;
	  
		/* behaviour similar to Unix stty ICRNL */
		if (c == '\r')
			c = '\n';
		if (c == '\n')
		{
			*cp = c;
			uart_putchar(c, stream);
			rxp = b;
			break;
		}
		else if (c == '\t')
			c = ' ';

		if ((c >= (uint8_t)' ' && c <= (uint8_t)'\x7e') || c >= (uint8_t)'\xa0')
		{
			if (cp == b + RX_BUFSIZE - 1)
				uart_putchar('\a', stream);
			else
			{
				*cp++ = c;
				uart_putchar(c, stream);
			}
			continue;
		}

		switch (c)
		{
			case 'c' & 0x1f:
				return -1;

			case '\b':
			case '\x7f':
				if (cp > b)
				{
					uart_putchar('\b', stream);
					uart_putchar(' ', stream);
					uart_putchar('\b', stream);
					cp--;
				}
				break;

			case 'r' & 0x1f:
				uart_putchar('\r', stream);
				for (cp2 = b; cp2 < cp; cp2++)
					uart_putchar(*cp2, stream);
				break;

			case 'u' & 0x1f:
				while (cp > b)
				{
					uart_putchar('\b', stream);
					uart_putchar(' ', stream);
					uart_putchar('\b', stream);
					cp--;
				}
				break;

			case 'w' & 0x1f:
				while (cp > b && cp[-1] != ' ')
				{
					uart_putchar('\b', stream);
					uart_putchar(' ', stream);
					uart_putchar('\b', stream);
					cp--;
				}
				break;
		}
	}

	c = *rxp++;
	if (c == '\n')
		rxp = 0;

	return c;
}