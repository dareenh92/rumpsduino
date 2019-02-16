#include "turbo_rumps.h"

const unsigned char intrlv_P[r_M] = {39, 26, 221, 22};

const unsigned char deintrlv_P[r_M] = {214, 199, 194, 181};

// Set encoder trellis
void set_trellis(){
	/*
	 	RSC encoder structure:
	 	- K=4
	 	- G1=13, G2=15, feedback=13
	*/
	trel.numStates = 8;
	
	trel.nextStates[0][0] = 0; trel.nextStates[0][1] = 4;
	trel.nextStates[1][0] = 4; trel.nextStates[1][1] = 0;
	trel.nextStates[2][0] = 5; trel.nextStates[2][1] = 1;
	trel.nextStates[3][0] = 1; trel.nextStates[3][1] = 5;
	trel.nextStates[4][0] = 2; trel.nextStates[4][1] = 6;
	trel.nextStates[5][0] = 6; trel.nextStates[5][1] = 2;
	trel.nextStates[6][0] = 7; trel.nextStates[6][1] = 3;
	trel.nextStates[7][0] = 3; trel.nextStates[7][1] = 7;
	
	trel.outputs[0] = 0; trel.outputs[1] = 3;
	trel.outputs[2] = 0; trel.outputs[3] = 3;
	trel.outputs[4] = 1; trel.outputs[5] = 2;
	trel.outputs[6] = 1; trel.outputs[7] = 2;
	trel.outputs[8] = 1; trel.outputs[9] = 2;
	trel.outputs[10] = 1; trel.outputs[11] = 2;
	trel.outputs[12] = 0; trel.outputs[13] = 3;
	trel.outputs[14] = 0; trel.outputs[15] = 3;
	
	trel.prevStates[0][0] = 0; trel.prevStates[0][1] = 1;
	trel.prevStates[1][0] = 3; trel.prevStates[1][1] = 2;
	trel.prevStates[2][0] = 4; trel.prevStates[2][1] = 5;
	trel.prevStates[3][0] = 7; trel.prevStates[3][1] = 6;
	trel.prevStates[4][0] = 1; trel.prevStates[4][1] = 0;
	trel.prevStates[5][0] = 2; trel.prevStates[5][1] = 3;
	trel.prevStates[6][0] = 5; trel.prevStates[6][1] = 4;
	trel.prevStates[7][0] = 6; trel.prevStates[7][1] = 7;
}

//********************************************
// Convolutionally encode data
// return the codeword in parameter, and
// return the final state of encoder
char convenc(unsigned char* seq, size_t dlen, unsigned char* codeword, unsigned char start_state){
	unsigned char curr_state = start_state;
	
	for(int i=0; i<dlen; i++){
		// codeword outputs	
		unsigned char u = (trel.outputs[(curr_state<<1)+seq[i]] & (2))>>1;
		unsigned char v = trel.outputs[(curr_state<<1)+seq[i]] & (1);
		
		codeword[(i<<1)] = u;
		codeword[(i<<1)+1] = v;
		
		curr_state = trel.nextStates[curr_state][seq[i]];
	}
	
	return curr_state;
}

//********************************************
// Turbo encoder
// seq	: as input, is original data sequence (WITHOUT termination bits)
// N		: length of original sequence (WITHOUT termination bits)
// codeword	: resulting codeword, sized N*3
void r_turbo_encode(unsigned char* seq, size_t dlen, unsigned char* codeword){
	/* trellis description of encoder
	two identical RSC encoders:
	- K=4
	- G1=13, G2=15, feedback=13
	
	Total code rate of 1/3, no puncturing 
	trellis are not zero-terminated
	DRP interleaver is used
	*/
	
	// vector for each encoder's resulting codeword
	unsigned char codeword_enc1[(dlen<<1)];
	unsigned char codeword_enc2[(dlen<<1)];
	
	/// First Encoder
	convenc(seq, dlen, codeword_enc1, 0);
	
	// interleave sequence
	unsigned char seq_i[dlen]; 
	drpintrlv_uc(seq, seq_i);
	
	// Second Encoder	
	convenc(seq_i, dlen, codeword_enc2, 0);
	
	/**************************
	* MUX
	**************************/	
	for(int i=0; i<dlen; i++){
		// systematic bit
		codeword[i+(i<<1)] = codeword_enc1[(i<<1)];
		
		// parity bit - punctured
    codeword[i+(i<<1)+1] = codeword_enc1[(i<<1)+1];
    codeword[i+(i<<1)+2] = codeword_enc2[(i<<1)+1];
	}
	
	/*
	printf("\nCodeword enc1\n");
	for(int i=0; i<dlen*2; i++)
		printf("%d ", codeword_enc1[i]);
	
	printf("\nCodeword enc2\n");
	for(int i=0; i<dlen*2; i++)
		printf("%d ", codeword_enc2[i]);*/
	//*****************************
}

//********************************************
// Part of Turbo Decoder
// -deMUX incoming seq to seqs for dec1 and dec2
// -done for every 3 bits of incoming seq
// -i keeps track on index of received seq (0, 1, .. ,K)
void r_turbodec_demux(accum recv_seq[3], accum* seq_dec1, accum* seq_dec2, unsigned char i){
	
	//note that seq_dec2's systematic bits are not interleaved
	//we interleave that after the whole sequence has been received
	//meanwhile we can still go through SW decoding for dec1
	
	seq_dec1[(i<<1)] = seq_dec2[(i<<1)] = recv_seq[0];
	seq_dec1[(i<<1)+1] = recv_seq[1];
	seq_dec2[(i<<1)+1] = recv_seq[2];
}

//********************************************
// Part of Turbo Decoder
// LUT Log-MAP's max* function 
accum max_f(accum a, accum b){
	//find max(a,b) and |a-b|
	accum ret_value = a;
	accum diff = a-b;
	if(diff<0){
		ret_value = b;
		diff *= -1;
	}
	
	//LUT for correction value
	/*if(diff==0)
		ret_value += 0.6931;
	else if(diff==0.5)
		ret_value += 0.474;
	else if(diff==1)
		ret_value += 0.3132;
	else if(diff==1.5)
		ret_value += 0.2014;
	else if(diff==2)
		ret_value += 0.1269;
	else if(diff==2.5)
		ret_value += 0.0789;
	else if(diff==3)
		ret_value += 0.0486;
	else if(diff==4)
		ret_value += 0.0181;*/
		
	//return ret_value + log(1+exp(-diff));	
	return ret_value;
}

//********************************************
// Part of Turbo Decoder
// -calc branch metric delta for one timestamp 
void r_turbodec_dcalc(accum recv_sys, accum recv_par, accum delta[16], accum apriori, accum noise_var){

  // iterates through possible state transitions
  for(char i=0; i<(trel.numStates<<1); i++){
		// get trellis output prototype, convert to bipolar
		char u = (trel.outputs[i] & (2))>>1;
		char v = trel.outputs[i] & (1);
		u = u+u-1;
		v = v+v-1;
		
		// recall apriori is P(x=1)
		if((i%2)==0)
			delta[i] = ln_approx(1-apriori) + (recv_sys*u + recv_par*v)/noise_var;
		else
			delta[i] = ln_approx(apriori) + (recv_sys*u + recv_par*v)/noise_var;
  }  
}

//********************************************
// Part of Turbo Decoder
// -calc LLR / Le for one timestamp
// -isLLR=1 means output LLR instead of Le
accum r_turbodec_llrcalc(accum curr_delta[16], accum curr_alpha[8], accum nxt_beta[8], accum recv_parity_bit, char isLLR, accum noise_var){
	accum LLR_Le_num = 0;
	accum LLR_Le_denom = 0;
	accum ratio = 0;
	char nextState, v;
	
	for(char i=0; i<trel.numStates; i++){
		//numerator part - bit 1
		nextState = trel.nextStates[i][1];
		v = trel.outputs[(i<<1) + 1] & (1);
		v = v+v-1;
    
    if(i==0){
			if(isLLR==1)
				LLR_Le_num = curr_alpha[i] + nxt_beta[nextState] + curr_delta[(i<<1)+1];
      else 
				LLR_Le_num = curr_alpha[i] + nxt_beta[nextState] + recv_parity_bit*v/noise_var;
    }
    else{
      if(isLLR==1)
				LLR_Le_num = max_f( LLR_Le_num, curr_alpha[i] + nxt_beta[nextState] + curr_delta[(i<<1)+1] );
      else
				LLR_Le_num = max_f( LLR_Le_num, curr_alpha[i] + nxt_beta[nextState] + recv_parity_bit*v/noise_var );
    }
		
		//denominator part - bit 0
		nextState = trel.nextStates[i][0];
		v = trel.outputs[(i<<1)] & (1);
		v = v+v-1;
    
    if(i==0){
			if(isLLR==1)
				LLR_Le_denom = curr_alpha[i] + nxt_beta[nextState] + curr_delta[(i<<1)];
      else 
				LLR_Le_denom = curr_alpha[i] + nxt_beta[nextState] + recv_parity_bit*v/noise_var;
    }
    else{
      if(isLLR==1)
				LLR_Le_denom = max_f( LLR_Le_denom, curr_alpha[i] + nxt_beta[nextState] + curr_delta[(i<<1)] );
    	else
				LLR_Le_denom = max_f( LLR_Le_denom, curr_alpha[i] + nxt_beta[nextState] + recv_parity_bit*v/noise_var );
    }	
	}
	
	//clipping of value to avoid overflow
	ratio = LLR_Le_num - LLR_Le_denom;
	if(ratio>10)
		ratio = 10;
	else if(ratio<-10)
		ratio = -10;
	
	//change ratio -> prob for Le
	if(isLLR!=1){
		ratio = ratio2prob(exp_approx(ratio));
		if(ratio > 0.99990)
			ratio = 0.99990;
		else if(ratio < 0.0001)
			ratio = 0.0001;
	}
	
	return ratio;
}

//********************************************
// Part of Turbo Decoder
// calc forward metric alpha for one timestamp
void r_turbodec_acalc(accum curr_delta[16], accum curr_alpha[8], accum nxt_alpha[8]){
	accum alpha_sum = 0;
	for(char i=0; i<trel.numStates; i++){
		nxt_alpha[i] = max_f(curr_delta[(trel.prevStates[i][0]<<1)] + curr_alpha[trel.prevStates[i][0]],
												 curr_delta[(trel.prevStates[i][1]<<1)+1] + curr_alpha[trel.prevStates[i][1]]);
		if(i==0)
			alpha_sum = nxt_alpha[i];
		else
			alpha_sum = max_f(alpha_sum, nxt_alpha[i]);
	}
	//normalization
	for(char i=0; i<trel.numStates; i++) nxt_alpha[i]-=alpha_sum;
}

//********************************************
// Part of Turbo Decoder
// calc forward metric beta for one timestamp
void r_turbodec_bcalc(accum curr_delta[16], accum curr_beta[8], accum nxt_beta[8]){
	double beta_sum = 0;
	for(char i=0; i<trel.numStates; i++){
		nxt_beta[i] = max_f(curr_delta[(i<<1)] + curr_beta[trel.nextStates[i][0]],
												 curr_delta[(i<<1)+1] + curr_beta[trel.nextStates[i][1]]);
		if(i==0)
			beta_sum = nxt_beta[i];
		else
			beta_sum = max_f(beta_sum, nxt_beta[i]);
	}
	//normalization
	for(char i=0; i<trel.numStates; i++) nxt_beta[i]-=beta_sum;
}

//********************************************
// Convert unipolar to bipolar - char
void uni2bi(char* uni_seq, int N){
	for(int i=0; i<N; i++)
		uni_seq[i] = (uni_seq[i]<<1) - 1;
}

// Convert ratio to probability (for apriori bit probability)
accum ratio2prob(accum ratio){
	return ratio/(1+ratio);
}
