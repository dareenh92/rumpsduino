/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

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
*/

#ifndef Arduino_h
#define Arduino_h

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h> // FOR RUMPS401

// some libraries and sketches depend on this
// AVR stuff, assuming Arduino.h or WProgram.h
// automatically includes it...
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "binary.h"
#include "itoa.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

// Includes Atmel CMSIS
//#include <chip.h>

#include "rumps401.h"

#include "wiring_constants.h"

#define clockCyclesPerMicrosecond() ( SystemCoreClock / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (SystemCoreClock / 1000L) )
#define microsecondsToClockCycles(a) ( (a) * (SystemCoreClock / 1000000L) )
#define SysTick_Config() (SystemCoreClock / 1000000L)


#define GPIO_DATAIN             ((*(volatile unsigned long *) (0x50000000))) // Data Input value
#define GPIO_DATAOUT            ((*(volatile unsigned long *) (0x50000004))) // Data Output Register
#define GPIO_OEN_SET            ((*(volatile unsigned long *) (0x50000008))) // Output enable control (input)
#define GPIO_OEN_CLR            ((*(volatile unsigned long *) (0x5000000C))) // Output enable control (output)
#define GPIO_IF                 ((*(volatile unsigned long *) (0x50000010))) // Interrupt Flag
#define GPIO_IE_SET             ((*(volatile unsigned long *) (0x50000014))) // Interrupt enable control
#define GPIO_IE_CLR             ((*(volatile unsigned long *) (0x50000018))) // Interrupt enable control
#define GPIO_IT_SET             ((*(volatile unsigned long *) (0x5000001C))) // Interrupt type control
#define GPIO_IT_CLR             ((*(volatile unsigned long *) (0x50000020))) // Interrupt type control
#define GPIO_IP_SET             ((*(volatile unsigned long *) (0x50000024))) // Interrupt polarity control
#define GPIO_IP_CLR             ((*(volatile unsigned long *) (0x50000028))) // Interrupt polarity control
#define GPIO_BSET               ((*(volatile unsigned long *) (0x5000002C))) // Bootloader receive buffer 0
#define GPIO_BCLR               ((*(volatile unsigned long *) (0x50000030))) // Bootloader receive buffer 0
#define GPIO_BTGL               ((*(volatile unsigned long *) (0x50000034))) // Bootloader receive buffer 0

#define __NOP() __asm__("nop\n\t")// DELAY

void yield(void);

/* sketch */
// Core 0 (IO Control Core)
extern void setup( void ) ;
extern void loop( void ) ;

// Core 1 (Normal Core 0)
extern void setup1( void ) ;
extern void loop1( void ) ;

// Core 2 (Normal Core 1)
extern void setup2( void ) ;
extern void loop2( void ) ;

// Core 3 (DSP Core)
extern void setup3( void ) ;
extern void loop3( void ) ;

//#define NOT_A_PIN 0  // defined in pio.h/EPioType
#define NOT_A_PORT           0

#define NOT_AN_INTERRUPT -1

typedef void (*voidFuncPtr)( void ) ;

/* Define attribute */
#if defined   ( __CC_ARM   ) /* Keil uVision 4 */
    #define WEAK (__attribute__ ((weak)))
#elif defined ( __ICCARM__ ) /* IAR Ewarm 5.41+ */
    #define WEAK __weak
#elif defined (  __GNUC__  ) /* GCC CS */
    #define WEAK __attribute__ ((weak))
#endif



#ifdef __cplusplus
} // extern "C"

#include "WCharacter.h"
#include "WString.h"
#include "Tone.h"
#include "WMath.h"
#include "HardwareSerial.h"
#include "wiring_pulse.h"

#endif // __cplusplus

// Include board variant
#include "variant.h"

#include "wiring.h"
#include "wiring_digital.h"
#include "wiring_analog.h"
#include "wiring_shift.h"
#include "WInterrupts.h"

#endif // Arduino_h
