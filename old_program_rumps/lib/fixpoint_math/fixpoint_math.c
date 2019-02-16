/*
 * fixpoint_math.h
 *
 *  Created on: Oct 5, 2016
 *      Author: Dareen
 */

#include "fixpoint_math.h"

// just a debugging print function
/*void print_debug_accum(accum x){
  accum_int_t temp;
  temp.accum_cont = x;
  if(x<0){
    temp.int_cont = ((~temp.int_cont) + 0x1);
    trace_printf("-");
  }
  trace_printf("%d.%5.5d ", ((unsigned int)(temp.int_cont>>15) & 0xFFFF),
               (((unsigned int)(temp.int_cont & 0x7FFF)*100000+16384)/32768));
}*/

// exponent approximation - adapted from http://www.quinapalus.com/
// find for negative value!!!!
accum exp_approx(accum x){
  // specifically works on signed Q16.15 fixed point

  // we use this union since all operations are on integer type
  accum_int_t y_t, temp, x_t;
  x_t.accum_cont = x;

  // calculations
  // **for positive argument
  if(x>=0){
    // init y to 1.0
    y_t.int_cont = 0x8000;

    temp.int_cont = x_t.int_cont - 181705;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont<<=8;}

    temp.int_cont = x_t.int_cont - 90852;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont<<=4;}

    temp.int_cont = x_t.int_cont - 45426;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont<<=2;}

    temp.int_cont = x_t.int_cont - 22711;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont<<=1;}

    temp.int_cont = x_t.int_cont - 13287;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont+=y_t.int_cont>>1;}

    temp.int_cont = x_t.int_cont - 7310;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont+=y_t.int_cont>>2;}

    temp.int_cont = x_t.int_cont - 3860;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont+=y_t.int_cont>>3;}

    temp.int_cont = x_t.int_cont - 1985;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont+=y_t.int_cont>>4;}

    temp.int_cont = x_t.int_cont - 1009;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont+=y_t.int_cont>>5;}

    temp.int_cont = x_t.int_cont - 507;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont+=y_t.int_cont>>6;}

    temp.int_cont = x_t.int_cont - 255;
    if(temp.accum_cont>=0){ x_t.int_cont = temp.int_cont; y_t.int_cont+=y_t.int_cont>>7;}
  }
  // **for negative argument
  else{
    temp.int_cont = x_t.int_cont + 22711;
    if(temp.accum_cont>0){ y_t.accum_cont = exp_approx(temp.accum_cont); y_t.int_cont >>= 1; return y_t.accum_cont;}

    temp.int_cont = x_t.int_cont + 45426;
    if(temp.accum_cont>0){ y_t.accum_cont = exp_approx(temp.accum_cont); y_t.int_cont >>= 2; return y_t.accum_cont;}

    temp.int_cont = x_t.int_cont + 90852;
    if(temp.accum_cont>0){ y_t.accum_cont = exp_approx(temp.accum_cont); y_t.int_cont >>= 4; return y_t.accum_cont;}

    temp.int_cont = x_t.int_cont + 181705;
    if(temp.accum_cont>0){ y_t.accum_cont = exp_approx(temp.accum_cont); y_t.int_cont >>= 8; return y_t.accum_cont;}

    temp.int_cont = x_t.int_cont + 272557;
    if(temp.accum_cont>0){ y_t.accum_cont = exp_approx(temp.accum_cont); y_t.int_cont >>= 12; return y_t.accum_cont;}

    temp.int_cont = x_t.int_cont + 340695;
    if(temp.accum_cont>0){ y_t.accum_cont = exp_approx(temp.accum_cont); y_t.int_cont >>= 15; return y_t.accum_cont;}
  }
  return y_t.accum_cont;
}

// natural logarithm approximation - adapted from http://www.quinapalus.com/
accum ln_approx(accum x){
  // specifically works on signed Q16.15 fixed point

  // we use this union since all operations are on integer type
  accum_int_t y_t, temp, x_t;
  x_t.accum_cont = x;

  // init y to 0.0
  y_t.int_cont = 0x0;

  temp.int_cont = x_t.int_cont<<8;
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=181705;}

  temp.int_cont = x_t.int_cont<<4;
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=90852;}

  temp.int_cont = x_t.int_cont<<2;
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=45426;}

  temp.int_cont = x_t.int_cont<<1;
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=22711;}

  temp.int_cont = x_t.int_cont + (x_t.int_cont>>1);
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=13287;}

  temp.int_cont = x_t.int_cont + (x_t.int_cont>>2);
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=7310;}

  temp.int_cont = x_t.int_cont + (x_t.int_cont>>3);
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=3860;}

  temp.int_cont = x_t.int_cont + (x_t.int_cont>>4);
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=1985;}

  temp.int_cont = x_t.int_cont + (x_t.int_cont>>5);
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=1009;}

  temp.int_cont = x_t.int_cont + (x_t.int_cont>>6);
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=507;}

  temp.int_cont = x_t.int_cont + (x_t.int_cont>>7);
  if(temp.accum_cont < 1){ x_t.int_cont = temp.int_cont; y_t.int_cont-=255;}

  return y_t.accum_cont;
}

// sqrt function - S15.16 format - from https://github.com/chmike/fpsqrt
unsigned int S15_16_sqrt ( unsigned int x )
{
    uint32_t t, q, b, r;
    r = x;
    b = 0x40000000;
    q = 0;
    while( b > 0x40 )
    {
        t = q + b;
        if( r >= t )
        {
            r -= t;
            q = t + b; // equivalent to q += 2*b
        }
        r <<= 1;
        b >>= 1;
    }
    q >>= 8;
    return q;
}