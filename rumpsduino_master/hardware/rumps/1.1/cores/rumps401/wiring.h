/*
  Copyright (c) 2011 Arduino.  All right reserved.
  Copyright (c) 2013 by Paul Stoffregen <paul@pjrc.com> (delayMicroseconds)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef _WIRING_
#define _WIRING_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "Arduino.h"
/**
 *
 */
extern void initVariant( void ) ;
extern void init( void ) ;

/**
 * \brief Returns the number of milliseconds since the Arduino board began running the current program.
 *
 * This number will overflow (go back to zero), after approximately 50 days.
 *
 * \return Number of milliseconds since the program started (uint32_t)
 */
extern uint32_t millis( void ) ;

extern uint32_t micros( void ) ;

/**
 * \brief Pauses the program for the amount of time (in miliseconds) specified as parameter.
 * (There are 1000 milliseconds in a second.)
 *
 * \param dwMs the number of milliseconds to pause (uint32_t)
 */
extern void delay( uint32_t dwMs ) ;
extern void delayMicroseconds( uint32_t dwUs ) ;


/**
 * \brief Pauses the program for the amount of time (in microseconds) specified as parameter.
 *
 * \param dwUs the number of microseconds to pause (uint32_t)
 */

// static inline void delayMicroseconds(uint32_t) __attribute__((always_inline, unused));
// static inline void delayMicroseconds(uint32_t usec){
//     /*
//      * Based on Paul Stoffregen's implementation
//      * for Teensy 3.0 (http://www.pjrc.com/)
//      */
//     if (usec == 0) return;
//     uint32_t n = usec * (VARIANT_MCK / 3000000);
//     asm volatile(
//         "L_%=_delayMicroseconds:"       "\n\t"
//         "subs   %0, #1"                 "\n\t"
//         "bne    L_%=_delayMicroseconds" "\n"
//         : "+r" (n) :
//     );
// }

#ifdef __cplusplus
}
#endif

#endif /* _WIRING_ */
