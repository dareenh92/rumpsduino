//------------------------------------------------------------------------------
// Library
// Name				: drp_intrlv.h
// Purpose
// - provide function call for DRP interleaving
// - fixed configuration of interleaver
//		K=256  M=4  s=0  p=13
//		w = [1 3 0 2]
//		r = [3 0 1 2]

#ifndef __DRP_INTRLV_H_
#define __DRP_INTRLV_H_

#define r_K 256
#define r_M 4

// drp index calculator
unsigned char drp_idxcalc( short K, unsigned char M, unsigned char Iprev, unsigned char* P, unsigned char i);

// drp interleaver - char
void drpintrlv_uc(unsigned char* input_seq, unsigned char* intrlvd_seq);

// drp interleaver - float
void drpintrlv_f(float* input_seq, float* intrlvd_seq);

// drp deinterleaver - char
void drpdeintrlv(unsigned char* input_seq, unsigned char* deintrlvd_seq);

#endif /* __DRP_INTRLV_H_ */
