#ifndef __UART_H_
#define __UART_H_

// UART.C
//
// Generic software uart written in C, requiring a timer set to 3 times
// the baud rate, and two software read/write pins for the receive and
// transmit functions.
//
// * Received characters are buffered
// * putchar(), getchar(), kbhit() and flush_input_buffer() are available
// * There is a facility for background processing while waiting for input
//
// Colin Gittins, Software Engineer, Halliburton Energy Services
//
// The baud rate can be configured by changing the BAUD_RATE macro as
// follows:
//
// #define BAUD_RATE            19200.0
//
// The function init_uart() must be called before any comms can take place
//
// Interface routines required:
// 1. get_rx_pin_status()
//    Returns 0 or 1 dependent on whether the receive pin is high or low.
// 2. set_tx_pin_high()
//    Sets the transmit pin to the high state.
// 3. set_tx_pin_low()
//    Sets the transmit pin to the low state.
// 4. idle()
//    Background functions to execute while waiting for input.
// 5. timer_set( BAUD_RATE )
//    Sets the timer to 3 times the baud rate.
// 6. set_timer_interrupt( timer_isr )
//    Enables the timer interrupt.
//
// Functions provided:
// 1. void flush_input_buffer( void )
//    Clears the contents of the input buffer.
// 2. char kbhit( void )
//    Tests whether an input character has been received.
// 3. char getchar( void )
//    Reads a character from the input buffer, waiting if necessary.
// 4. void turn_rx_on( void )
//    Turns on the receive function.
// 5. void turn_rx_off( void )
//    Turns off the receive function.
// 6. void putchar( char )
//    Writes a character to the serial port.
 
#include <stdio.h>
 
//#define BAUD_RATE       115200.0
#define IN_BUF_SIZE     256
 
#define TRUE 1
#define FALSE 0

#define BAUD_SPLIT 3

#ifndef UART_TX
#define UART_TX 0	// GPIO 0
#endif

#ifndef UART_RX
#define UART_RX 1 // GPIO 1
#endif

////////////////////////////////
// Interface routines required:
// (Platform dependent)
////////////////////////////////
char get_rx_pin_status(void);
void set_tx_pin_high(void);
void set_tx_pin_low(void);
void idle(void);
void timer_set(void);

////////////////////////////
// Generic Function
////////////////////////////
void timer_isr(void);
void init_uart( void );
//char _getchar( void );
char _getchar( char *ch );
void _putchar( char ch );
void flush_input_buffer( void );
char kbhit( void );
void turn_rx_on( void );
void turn_rx_off( void );

#endif
