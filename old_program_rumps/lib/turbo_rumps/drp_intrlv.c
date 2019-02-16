//------------------------------------------------------------------------------
// Library
// Name				: drp_intrlv.c

#include "drp_intrlv.h"

// drp index calculator
unsigned char drp_idxcalc( short K, unsigned char M, unsigned char Iprev, unsigned char* P, unsigned char i){
	return (Iprev + P[i&(M-1)]) & (K-1);
}

// drp interleaver - char
void drpintrlv_uc(unsigned char* input_seq, unsigned char* intrlvd_seq){
  unsigned char P[r_M] = {39, 26, 221, 22};
  unsigned char Im1 = 229;
  unsigned char p_idx = Im1;
  
  for(unsigned short i=0; i<r_K; i++){
    p_idx = drp_idxcalc( r_K, r_M, p_idx, P, i);
    intrlvd_seq[i] = input_seq[p_idx];
	}
}

// drp interleaver - float
void drpintrlv_f(float* input_seq, float* intrlvd_seq){
  unsigned char P[r_M] = {39, 26, 221, 22};
  unsigned char Im1 = 229;
  unsigned char p_idx = Im1;
  
  for(unsigned short i=0; i<r_K; i++){
    p_idx = drp_idxcalc( r_K, r_M, p_idx, P, i);
    intrlvd_seq[i] = input_seq[p_idx];
	}
}

// drp deinterleaver - char
void drpdeintrlv(unsigned char* input_seq, unsigned char* deintrlvd_seq){
  unsigned char P[r_M] = {214, 199, 194, 181};
  unsigned char Im1 = 238;
  unsigned char p_idx = Im1;
  
  for(unsigned short i=0; i<r_K; i++){
    p_idx = drp_idxcalc( r_K, r_M, p_idx, P, i);
    deintrlvd_seq[i] = input_seq[p_idx];
	}
}
