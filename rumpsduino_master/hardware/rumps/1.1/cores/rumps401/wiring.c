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

    #include<stdio.h>
    #include<stdlib.h>
    #include<math.h>
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
    //Perlu tes lompat fungsi makan waktu ga
    delayMicroseconds(ms*1000);
}

// void delayMicroseconds( uint32_t us )
// {
//     uint32_t tes=us-(us*0.153);
//     if(us <= 1000) tes=us;
//     else tes=us-(us*0.162);
//     // int i=0;
//     while(tes--!=0){
//         __NOP();
//         // us--;
//     }
//     // do{
//     //   __NOP();
//     // }while(tes--!=0);
//     // // uint32_t i=0;
//     // for(i=0;i<tes;i++){
//     //     __NOP();
//     // }
    
// }

void delayMicroseconds(uint32_t us)
{
    if (us > 55){
        us -= 34; // Karena if di bawah makan 35 us

        if (us < 32768) us = us * us / (((us * 19) >> 4) + 2);
        else us = us / 19 * 16;
    } else us -= us >> 2;

    //Delay with padding
    asm("ldr  r0, %[us]\n"      // 2 cycle
    "lop: sub  r0, r0, #1\n" // 1 cycle
    "nop\n"
    "cmp r0, #0\n"         // 1 cycle
    "nop\n" //paddings
    "bne  lop\n" 
    "nop\n" :: [us]"m" (us):); //2 cycle 
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
