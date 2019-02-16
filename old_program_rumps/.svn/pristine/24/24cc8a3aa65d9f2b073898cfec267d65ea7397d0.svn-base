//------------------------------------------------------------------------------
// Library
// Name					: turbo_rumps.h
// Core-specific: DSP core 
// Purpose
//	- turbo encoding & decoding, fixed configuration
//	- two identical RSC encoders:
//	 	K=4
//	 	G1=13, G2=15, feedback=13
//	- Both encoders are not trellis terminated
//	- Log-MAP algorithm
//	- functions are made as general as possible

#ifndef __TURBO_RUMPS_C3_H_
#define __TURBO_RUMPS_C3_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdfix.h>
#include "fixpoint_math.h"
//#include "drp_intrlv.h"

#define r_win 32
#define r_win_mul 5
#define intrlv_Im1 229
#define deintrlv_Im1 238
#define r_M 4

//#define max_f(x, y) ((x) > (y) ? (x) : (y))

// create trellis structure
typedef struct {
	
	unsigned char numStates;
	unsigned char nextStates [8][2];
	unsigned char outputs [16];
	unsigned char prevStates [8][2];
}trellis;

// Set trellis based on defined spec
void set_trellis();

// Decoder - Log-MAP's max* function 
accum max_f(accum a, accum b);

// Decoder - calc branch metric delta
void r_turbodec_dcalc(accum recv_sys, accum recv_par, accum delta[16], accum ln_apriori1, accum ln_apriori0, accum noise_var);

// Decoder - calc LLR / Le
accum r_turbodec_llrcalc(accum curr_delta[16], accum curr_alpha[8], accum nxt_beta[8], accum recv_parity_bit, char isLLR, accum noise_var);

// Convert ratio to probability (for apriori bit probability)
accum ratio2prob(accum ratio);

//------------------------------------------------------------------------------
// Global variables
extern trellis trel;

extern const unsigned char intrlv_P[r_M];

extern const unsigned char deintrlv_P[r_M];

#endif /* __TURBO_RUMPS_C3_H_ */
