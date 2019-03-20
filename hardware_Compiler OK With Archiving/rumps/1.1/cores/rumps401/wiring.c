/*
  Copyright (c) 2011 Arduino.  All right reserved.

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

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

/*void delay( uint32_t ms )
{
    if (ms == 0)
        return;
    uint32_t start = GetTickCount();
    do {
        yield();
    } while (GetTickCount() - start < ms);
}*/

//FOR RUMPS
// 500k=1/2s, 1000k=1s
void delay( uint32_t ms )
{
    int i = 0;
    for (i = 0; i < (ms*1000); i++){
    __NOP();
  }
}


void delayMicroseconds( uint32_t us )
{
    int i = 0;
    for (i = 0; i < us; i++){
    __NOP();
  }
}

volatile uint32_t counter = 0;

void SysTick_Handler(void) {
  counter++;
}

uint32_t millis() {
  return counter;
}
uint32_t micros() {
  return counter*1000;
}



/* Delay dengan Systick
static uint32_t usTicks;
void SysTick_Handler()
{
    if (usTicks != 0)
    {
        usTicks--;
    }
}
void delayMicroseconds(uint32_t us)
{
    // Reload us value
    usTicks = us;
    // Wait until usTick reach zero
    while (usTicks);
}

void delay(uint32_t ms)
{
    // Wait until ms reach zero
    while (ms--)
    {
        // Delay 1ms
        delayMicroseconds(1000);
    }
}
*/
#if defined ( __ICCARM__ ) /* IAR Ewarm 5.41+ */
extern signed int putchar( signed int c ) ;
/**
 * \brief
 *
 * \param c  Character to output.
 *
 * \return The character that was output.
 */
extern WEAK signed int putchar( signed int c )
{
    return c ;
}
#endif /* __ICCARM__ */

#ifdef __cplusplus
}
#endif
