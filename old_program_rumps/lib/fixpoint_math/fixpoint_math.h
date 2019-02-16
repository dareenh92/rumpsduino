/*
 * fixpoint_math.h
 *
 *  Created on: Oct 5, 2016
 *      Author: Dareen
 */

#ifndef __FIXPOINT_MATH_H_
#define __FIXPOINT_MATH_H_

#include <stdfix.h>
#include <stdint.h>

#define sign_f(num) ((num != 0) | (num >> 31))  // -1, 0, or +1
#define abs_f(num) ((num + (num>>31)) ^ (num>>31) ) // absolute value

// type for accum and int counterpart
typedef union accum_int_manip{
  accum accum_cont;
  int int_cont;
} accum_int_t;

typedef union ulaccum_int_manip{
  unsigned long accum ulaccum_cont;
  uint64_t uint_cont;
} ulaccum_int_t;

// just a debugging print function
//void print_debug_accum(accum x);

// mul and div function for accum data type, utilizing MAC hardware of RUMPS
//inline accum rumps_accum_mul_s32(accum a, accum b){}

// exponent approximation - adapted from http://www.quinapalus.com/
// find for negative value!!!!
accum exp_approx(accum x);

// natural logarithm approximation - adapted from http://www.quinapalus.com/
accum ln_approx(accum x);

// sqrt function - S15.16 format - from https://github.com/chmike/fpsqrt
unsigned int S15_16_sqrt ( unsigned int x );



#endif /* __FIXPOINT_MATH_H_ */
