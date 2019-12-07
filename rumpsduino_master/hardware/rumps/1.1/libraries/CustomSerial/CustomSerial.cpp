/*
CustomSerial.cpp (formerly NewSoftSerial.cpp) -
Multi-instance software serial library for Arduino/Wiring
-- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
-- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and
   direct port writing by Mikal Hart (http://www.arduiniana.org)
-- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
-- 20MHz processor support by Garrett Mace (http://www.macetech.com)
-- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

The latest version of this library can always be found at
http://arduiniana.org.
*/

//DEFINE
#define uart_hd_recv_bit(inbuf) { \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	if (GPIO_DATAIN & UART_HD_RX){ \
	  __NOP(); \
	  __NOP(); \
	  \
	  inbuf = 0x1; \
	} \
    else { \
  	  inbuf = 0x0; \
    } \
	\
}


//--------------------------------------------------------------
#define uart_hd_recv_bit115200(inbuf) { \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	__NOP(); \
	\
	if (GPIO_DATAIN & UART_HD_RX){ \
	  __NOP(); \
	  __NOP(); \
	  \
	  inbuf = 0x1; \
	} \
    else { \
  	  inbuf = 0x0; \
    } \
	\
}

//END OF DEFINE

#ifndef SHORTEN_UART_HD
#define uart_hd_send() { \
	\
	GPIO_BCLR = UART_HD_TX;	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done\n" \
		"uhd_tx_z:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done:\n" \
			"nop" \
		:: [input0] "m" (outbuf0), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z1\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done1\n" \
		"uhd_tx_z1:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done1:\n" \
			"nop" \
		:: [input0] "m" (outbuf1), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z2\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done2\n" \
		"uhd_tx_z2:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done2:\n" \
			"nop" \
		:: [input0] "m" (outbuf2), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z3\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done3\n" \
		"uhd_tx_z3:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done3:\n" \
			"nop" \
		:: [input0] "m" (outbuf3), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z4\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done4\n" \
		"uhd_tx_z4:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done4:\n" \
			"nop" \
		:: [input0] "m" (outbuf4), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z5\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done5\n" \
		"uhd_tx_z5:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done5:\n" \
			"nop" \
		:: [input0] "m" (outbuf5), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z6\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done6\n" \
		"uhd_tx_z6:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done6:\n" \
			"nop" \
		:: [input0] "m" (outbuf6), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z7\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done7\n" \
		"uhd_tx_z7:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done7:\n" \
			"nop" \
		:: [input0] "m" (outbuf7), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	GPIO_BSET = UART_HD_TX;	\
}
#else
#define uart_hd_send() { \
	__NOP(); \
}
#endif

#define uart_hd_send115200() { \
	volatile int i; \
	\
	GPIO_BCLR = UART_HD_TX;	\
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done_115200\n" \
		"uhd_tx_z_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf0), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z1_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done1_115200\n" \
		"uhd_tx_z1_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done1_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf1), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z2_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done2_115200\n" \
		"uhd_tx_z2_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done2_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf2), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z3_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done3_115200\n" \
		"uhd_tx_z3_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done3_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf3), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z4_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done4_115200\n" \
		"uhd_tx_z4_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done4_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf4), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z5_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done5_115200\n" \
		"uhd_tx_z5_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done5_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf5), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z6_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done6_115200\n" \
		"uhd_tx_z6_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done6_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf6), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z7_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done7_115200\n" \
		"uhd_tx_z7_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done7_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf7), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	GPIO_BSET = UART_HD_TX;	\
}

// :: [input0] "m" (outbuf0) :

//--------------------------------------------------------------
#ifndef SHORTEN_UART_HD
#define uart_hd_recv() { \
	\
	while (GPIO_DATAIN & UART_HD_RX); \
	\
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
	uart_hd_recv_bit(inbuf0); \
	uart_hd_recv_bit(inbuf1); \
	uart_hd_recv_bit(inbuf2); \
	uart_hd_recv_bit(inbuf3); \
	uart_hd_recv_bit(inbuf4); \
	uart_hd_recv_bit(inbuf5); \
	uart_hd_recv_bit(inbuf6); \
	uart_hd_recv_bit(inbuf7); \
    \
    uart_hd_recv_bit(stop); \
}
#else
#define uart_hd_recv() { \
	__NOP(); \
}
#endif



//--------------------------------------------------------------
#define uart_hd_recv115200() { \
	volatile int i; \
	\
	while (GPIO_DATAIN & UART_HD_RX); \
	\
	GPIO_BTGL = 0x00000004; \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	uart_hd_recv_bit115200(inbuf0); \
	uart_hd_recv_bit115200(inbuf1); \
	uart_hd_recv_bit115200(inbuf2); \
	uart_hd_recv_bit115200(inbuf3); \
	uart_hd_recv_bit115200(inbuf4); \
	uart_hd_recv_bit115200(inbuf5); \
	uart_hd_recv_bit115200(inbuf6); \
	uart_hd_recv_bit115200(inbuf7); \
    \
    uart_hd_recv_bit115200(stop); \
}

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <Arduino.h>
#include "CustomSerial.h"
#include <math.h>
//
// Statics
//
// CustomSerial *CustomSerial::active_object = 0;


// #define UART_HD_TX 0x00020000 //17

// #define UART_HD_RX 0x00010000 //16


//
// Constructor
//
static char UART_HD_BAUD;

uint32_t CustomSerial::access_pin(uint32_t ulPin)
{
  if(ulPin>=32 && ulPin<40)
  {
    ulPin=ulPin-32;
  }
  else if(ulPin>=40 && ulPin<48)
  {
    ulPin=ulPin-40;
  }
  else if(ulPin>=48)
  {
    ulPin=ulPin-48;
  }
  
  return ulPin;
}

CustomSerial::CustomSerial(char receivePin, uint8_t transmitPin)
{
  // uint32_t rcv = access_pin(receivePin);
  // UART_HD_RX = 0x1 << rcv;
  // uint32_t trs = access_pin(transmitPin);
  // UART_HD_TX = 0x1 << trs;

	// GPIO_BSET = 0x1 << trs;		// TX high during IDLE
	
	// GPIO_OEN_CLR = 0x1 << rcv;	// set GPIO pin as input (RX)
	// GPIO_OEN_SET = 0x1 << trs;	// set GPIO pin as output (TX)
	UART_HD_BAUD = receivePin;

	GPIO_BSET = UART_HD_TX;		// TX high during IDLE
	
	GPIO_OEN_CLR = UART_HD_RX;	// set GPIO pin as input (RX)
	GPIO_OEN_SET = UART_HD_TX;
}

//
// Public methods
//

void CustomSerial::begin(char speed)
{
    UART_HD_BAUD = 1;
}

// #define uart_hd_poll_recv()
// #define uart_hd_poll_recv115200()

// #define uart_hd_poll_send()
// #define uart_hd_poll_send115200()


char CustomSerial::_getchar()
{
    unsigned char inbuf;

	volatile unsigned char inbuf0;
	volatile unsigned char inbuf1;
	volatile unsigned char inbuf2;
	volatile unsigned char inbuf3;

	volatile unsigned char inbuf4;
	volatile unsigned char inbuf5;
	volatile unsigned char inbuf6;
	volatile unsigned char inbuf7;

	volatile unsigned char stop;

	// invoke_UART_receive
	if (UART_HD_BAUD == 0){
	  uart_hd_recv();
	}
	else{
	  uart_hd_recv115200();
	}

	inbuf = inbuf0;
	inbuf |= (inbuf1 << 1);
	inbuf |= (inbuf2 << 2);
	inbuf |= (inbuf3 << 3);
	inbuf |= (inbuf4 << 4);
	inbuf |= (inbuf5 << 5);
	inbuf |= (inbuf6 << 6);
	inbuf |= (inbuf7 << 7);
    	
  return inbuf;

}

void CustomSerial::_putchar(char ch)
{
  volatile unsigned int outbuf0;
	volatile unsigned int outbuf1;
	volatile unsigned int outbuf2;
	volatile unsigned int outbuf3;

	volatile unsigned int outbuf4;
	volatile unsigned int outbuf5;
	volatile unsigned int outbuf6;
	volatile unsigned int outbuf7;

	volatile unsigned int stop;

	volatile unsigned int tx;

	outbuf0 = (ch & 0x1);
	outbuf1 = (ch & 0x2);
	outbuf2 = (ch & 0x4);
	outbuf3 = (ch & 0x8);

	outbuf4 = (ch & 0x10);
	outbuf5 = (ch & 0x20);
	outbuf6 = (ch & 0x40);
	outbuf7 = (ch & 0x80);

	stop = 0x1;

	tx = UART_HD_TX;
 
	//invoke_UART_transmit
	if (UART_HD_BAUD == 0){
	  uart_hd_send();
	}
	else{
	  uart_hd_send115200();
	}
}


void CustomSerial::print(const char* print_string){
	int i=0;
	
	while(print_string[i]!='\0')
		CustomSerial::_putchar(print_string[i++]); //put string
}

void CustomSerial::println(const char* print_string){
	int i=0;
	
	while(print_string[i]!='\0')
		CustomSerial::_putchar(print_string[i++]); //put string
		
	CustomSerial::_putchar(0xa); //put newline character
	CustomSerial::_putchar(0xd); //put carriage return character
}

int CustomSerial::peek()
{
  return 1;
}

void CustomSerial::flush()
{
  
}

// Read data from buffer
int CustomSerial::read()
{
  return 1;
}

int CustomSerial::available()
{
  return 1;
}

size_t CustomSerial::write(uint8_t b)
{
  
}