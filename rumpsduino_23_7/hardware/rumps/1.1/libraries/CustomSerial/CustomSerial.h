/*
CustomSerial.h (formerly NewSoftSerial.h) -
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

#ifndef CustomSerial_h
#define CustomSerial_h
// DELAY


#include <inttypes.h>
#include <Stream.h>
#include <stdio.h>
#include "Arduino.h"


/******************************************************************************
* Definitions
******************************************************************************/


class CustomSerial : public Stream
{
private:
  // per object data
  // static CustomSerial *active_object;

  uint32_t access_pin(uint32_t ulPin);

public:
  // public methods
  CustomSerial(char receivePin, uint8_t transmitPin);
  void begin(char speed);
  int peek();

  virtual void _putchar(char ch);
  virtual char _getchar();
  virtual void print(const char* pr_string);
  virtual void println(const char* pr_string);
  
  virtual size_t write(uint8_t byte);
  virtual int read();
  virtual int available();
  virtual void flush();
  using Print::write;
};

// Arduino 0012 workaround
#undef int
#undef char
#undef long
#undef byte
#undef float
#undef abs
#undef round

#endif
