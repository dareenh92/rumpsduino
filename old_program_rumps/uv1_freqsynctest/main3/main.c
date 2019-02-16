//------------------------------------------------------------------------------

//

// Main Program
// Application    : freqsync_test_dspc
// Core           : DSP Core
// Purpose
//  - Sine LUT test 
//  - Freq sync test
//  - Both above are using Accum and RUMPS' MAC 

// ###  ###

#include "main.h"
#include "fixpoint_math.h"

// LED pin number
const unsigned char ledpin = 0;

// Frequency sync known-preamble
const accum freqsync_preamble[2] = { // since I=Q, this saves space
  0.707,   // 1st preamble - 1
  -0.707  // 2nd preamble - 0
};

// Mapping constant for sine-cos lookup
const unsigned long accum sineMappingConst = 0.0012437;

// Sine look up table
const accum sineTable[256] = {
  0.0000, 0.0245, 0.0491, 0.0736, 0.0980, 0.1224, 0.1467, 0.1710,
  0.1951, 0.2191, 0.2430, 0.2667, 0.2903, 0.3137, 0.3369, 0.3599,
  0.3827, 0.4052, 0.4276, 0.4496, 0.4714, 0.4929, 0.5141, 0.5350,
  0.5556, 0.5758, 0.5957, 0.6152, 0.6344, 0.6532, 0.6716, 0.6895,
  0.7071, 0.7242, 0.7410, 0.7572, 0.7730, 0.7883, 0.8032, 0.8176,
  0.8315, 0.8449, 0.8577, 0.8701, 0.8819, 0.8932, 0.9040, 0.9142,
  0.9239, 0.9330, 0.9415, 0.9495, 0.9569, 0.9638, 0.9700, 0.9757,
  0.9808, 0.9853, 0.9892, 0.9925, 0.9952, 0.9973, 0.9988, 0.9997,
  1.0000, 0.9997, 0.9988, 0.9973, 0.9952, 0.9925, 0.9892, 0.9853,
  0.9808, 0.9757, 0.9700, 0.9638, 0.9569, 0.9495, 0.9415, 0.9330,
  0.9239, 0.9142, 0.9040, 0.8932, 0.8819, 0.8701, 0.8577, 0.8449,
  0.8315, 0.8176, 0.8032, 0.7883, 0.7730, 0.7572, 0.7410, 0.7242,
  0.7071, 0.6895, 0.6716, 0.6532, 0.6344, 0.6152, 0.5957, 0.5758,
  0.5556, 0.5350, 0.5141, 0.4929, 0.4714, 0.4496, 0.4276, 0.4052,
  0.3827, 0.3599, 0.3369, 0.3137, 0.2903, 0.2667, 0.2430, 0.2191,
  0.1951, 0.1710, 0.1467, 0.1224, 0.0980, 0.0736, 0.0491, 0.0245,
  0.0000, -0.0245, -0.0491, -0.0736, -0.0980, -0.1224, -0.1467, -0.1710,
  -0.1951, -0.2191, -0.2430, -0.2667, -0.2903, -0.3137, -0.3369, -0.3599,
  -0.3827, -0.4052, -0.4276, -0.4496, -0.4714, -0.4929, -0.5141, -0.5350,
  -0.5556, -0.5758, -0.5957, -0.6152, -0.6344, -0.6532, -0.6716, -0.6895,
  -0.7071, -0.7242, -0.7410, -0.7572, -0.7730, -0.7883, -0.8032, -0.8176,
  -0.8315, -0.8449, -0.8577, -0.8701, -0.8819, -0.8932, -0.9040, -0.9142,
  -0.9239, -0.9330, -0.9415, -0.9495, -0.9569, -0.9638, -0.9700, -0.9757,
  -0.9808, -0.9853, -0.9892, -0.9925, -0.9952, -0.9973, -0.9988, -0.9997,
  -1.0000, -0.9997, -0.9988, -0.9973, -0.9952, -0.9925, -0.9892, -0.9853,
  -0.9808, -0.9757, -0.9700, -0.9638, -0.9569, -0.9495, -0.9415, -0.9330,
  -0.9239, -0.9142, -0.9040, -0.8932, -0.8819, -0.8701, -0.8577, -0.8449,
  -0.8315, -0.8176, -0.8032, -0.7883, -0.7730, -0.7572, -0.7410, -0.7242,
  -0.7071, -0.6895, -0.6716, -0.6532, -0.6344, -0.6152, -0.5957, -0.5758,
  -0.5556, -0.5350, -0.5141, -0.4929, -0.4714, -0.4496, -0.4276, -0.4052,
  -0.3827, -0.3599, -0.3369, -0.3137, -0.2903, -0.2667, -0.2430, -0.2191,
  -0.1951, -0.1710, -0.1467, -0.1224, -0.0980, -0.0736, -0.0491, -0.0245
};

// sine lookup table, for details look at note
// RUMPS and n1169 extension specific
inline accum sin_lookup(int inAngle){
  int idx = 0;
  int angleSign = sign_f(inAngle);
  
  // Filter input angle
  if(angleSign<0) // sin(-x) = -sin(x)
    inAngle = (~inAngle)+1;

  // COMPILER_ERROR - assuming input won't exceed 2pi
  // let's discard this operation for now
  // while(inAngle>205887) // sin(x) = sin(x+2pi)
  //   inAngle-=205887;
  
  // Index mapping - inAngle to LUT index
  // simply want integer part of inAngle * mappingConst
  // implementation is a bit complicated due to 64bit mul
  ulaccum_int_t mappingInput,
                mappingConst;
  mappingInput.ulaccum_cont = inAngle;          // A
  mappingConst.ulaccum_cont = sineMappingConst; // B

  //  split a and b into 32 bits halves
  uint32_t a_lo = (uint32_t) mappingInput.uint_cont;
  uint32_t a_hi = mappingInput.uint_cont >> 32;
  uint32_t b_lo = (uint32_t) mappingConst.uint_cont;
  uint32_t b_hi = mappingConst.uint_cont >> 32;

  // * splitting calculations
  uint64_t p0 = mac_umul_32(a_lo, b_lo);
  uint64_t p1 = mac_umul_32(a_lo, b_hi);
  uint64_t p2 = mac_umul_32(a_hi, b_lo);
  uint64_t p3 = mac_umul_32(a_hi, b_hi);

  // * carry from lower half MUL
  uint32_t cy = (uint32_t)(((p0 >> 32) + (uint32_t)p1 + (uint32_t)p2) >> 32);

  // final MUL result - we only take hi (S31.32)
  // lo = p0 + (p1 << 32) + (p2 << 32);
  idx = p3 + (p1 >> 32) + (p2 >> 32) + cy;

  // Return lookup value
  // COMPILER_ERROR - if both modification and return happen
  // accum_int_t returnVal;
  // returnVal.accum_cont = sineTable[idx];  
  // if(angleSign<0) // sin(-x) = -sin(x)
  //   returnVal.int_cont = (~returnVal.int_cont) + 1;
  // return returnVal.accum_cont; 
  
  return angleSign * sineTable[idx];
}

// cosine lookup table, for details look at note
// RUMPS and n1169 extension specific
inline accum cos_lookup(int inAngle){
  int idx = 0;

  // Filter input angle
  if(sign_f(inAngle)<0) // cos(-x) = cos(x)
    inAngle = (~inAngle)+1;

  // Return lookup value
  // cos(x) = sin(x+pi/2)
  inAngle += 51472; // 51472 is pi/2 in accum format
  return sin_lookup(inAngle);
}

//------------------------------------------------------------------------------

int main(void)

{
  int lookupAngle;
  accum_int_t lookupResult;

  // *** Sine LUT test part ***
  
  while(1){
    // wait for angle as sine input (int rep of Accum)
    while(noc_NC_rxbuff0_av!=1)__NOP();
    lookupAngle = NC_NOC_RX_BUFF0;
    
    // If lookup value is valid (only to avoid COMPILER_ERROR)
    if(lookupAngle>=0){
      // // sine lookup
      lookupResult.accum_cont = sin_lookup(lookupAngle);

      // // send back LUT result
      while(noc_NC_txbuff_isfull==1)__NOP();
      NC_NOC_TX_BUFF0 = lookupResult.int_cont;
    }

    // Again, no idea why this 'break' is needed to
    // avoid COMPILER_ERROR
    else
      break;
  }

  // *** END - Sine LUT test ***


  // *** Freq sync test part ***
  /*
  while(1){

  }
  */
  // *** END - Freq sync test ***



  return 0;
}