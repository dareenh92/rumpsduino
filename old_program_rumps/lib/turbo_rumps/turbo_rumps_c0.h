//------------------------------------------------------------------------------
// Library
// Name					: turbo_rumps_c0.h 
// Core-specific: IO core 
// Purpose
//	- turbo encoding & decoding, fixed configuration
//	- two identical RSC encoders:
//	 	K=4
//	 	G1=13, G2=15, feedback=13
//	- Both encoders are not trellis terminated
//	- Log-MAP algorithm
//	- functions are made as general as possible

#ifndef __TURBO_RUMPS_C0_H_
#define __TURBO_RUMPS_C0_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdfix.h>
#include "fixpoint_math.h"
#include "drp_intrlv.h"

#define r_win 32
#define r_win_mul 5
#define intrlv_Im1 229
#define deintrlv_Im1 238

// create trellis structure
typedef struct trelstruct {
	
	unsigned char numStates;
	unsigned char nextStates [8][2];
	unsigned char outputs [16];
	unsigned char prevStates [8][2];
} trellis;

// Set trellis based on defined spec
void set_trellis();

// Convolutionally encode data
char convenc(unsigned char* seq, size_t dlen, unsigned char* codeword, unsigned char start_state);

// Encode data with turbo encoder structure
void r_turbo_encode(unsigned char* seq, size_t dlen, unsigned char* codeword);

//------------------------------------------------------------------------------
// Global variables
extern trellis trel;

extern const unsigned char intrlv_P[r_M];

extern const unsigned char deintrlv_P[r_M];

#endif /* __TURBO_RUMPS_C0_H_ */
