//------------------------------------------------------------------------------

//

// Main Program
// Application		: map_algo
// Core				: Any
// Purpose
//	- Testing MAP decoding algorithm on M0


#include "main.h"
#include <string.h>
#include <math.h>

#define euler_const 2.71828183

// create trellis structure
typedef struct {
	
	unsigned char numStates;
	unsigned char nextStates [4][2];
	unsigned char outputs [8];

}trellis;

// Convert unipolar to bipolar 
void uni2bi(char* uni_seq, int N);

// MAP decoding function
// Accepts sequence, apriori probability of bit 1, noise variance
void map_decode(char* seq, float* apriori_b1, float noise_var, size_t N, float* LLR);

void main_scheduler (void);

//------------------------------------------------------------------------------

int main(void){
	
	//////////////////
	// Data seq - Length 5
	//////////////////
	// data_tx = 1_1000
	char coded_seq1[10] = {1, 1, 1, 1, 0, 1, 0, 1, 0, 0};
	
	// data_tx = 0_0100
	char coded_seq2[10] = {0, 0, 0, 0, 1, 1, 0, 0, 0, 1};
	
	// data_tx = 1_0010
	char coded_seq3[10] = {1, 1, 0, 0, 0, 1, 1, 1, 0, 0};
	
	// data_tx = 0_1100
	//char coded_seq4[10] = {0, 0, 1, 1, 1, 1, 0, 1, 0, 1};
	
	// data_tx = 1_0100
	//char coded_seq5[10] = {1, 1, 0, 0, 1, 0, 0, 0, 0, 1};
	
	float apriori_b1 [10] = {.5, .5, .5, .5, .5, .5, .5, .5, .5, .5};
	
	//char coded_seq1[6] = {1, 1, 0, 0, 0, 1};
	//float apriori_b1 [6] = {.5, .5, .5, .5, .5, .5};
	
	char input_seq[10];
	
	// length of data(uncoded) -> simply multiply incoming data length by coderate (1/2)
	// equals to number of data timestamps
	size_t N = sizeof(input_seq)/sizeof(input_seq[0])/2;
	
	float LLR[N];
	
	// to store data received through serial
	char ch;
	
	char decoded_seq[N];
	
	for(int i=0; i<N; i++)
		decoded_seq[i] = 0;		
			
	//initialize uart
	init_uart();
	
	//main loop
	while(1){
		
		ch = _getchar();
		_putchar(ch);
		_putchar(N);
		
		if(ch=='a'){
			memcpy(&input_seq, coded_seq1, N*2* sizeof(char));
			//map_decode(input_seq, apriori_b1, 1, N, LLR);
		}
		else if(ch=='b'){		
			memcpy(&input_seq, coded_seq2, N*2* sizeof(char));
			//map_decode(input_seq, apriori_b1, 1, N, LLR);
		}
		else if(ch=='c'){
			memcpy(&input_seq, coded_seq3, N*2* sizeof(char));
			//map_decode(input_seq, apriori_b1, 1, N, LLR);
		}
		
		for(int i=0; i<N; i++){
			if(LLR[i]>0)
				decoded_seq[i] = 1;
			else if(LLR[i]<0)
				decoded_seq[i] = 0;
		
		}
		
		for(int i=0; i<N*2; i++)
			_putchar(input_seq[i]);	
		
		_putchar('#');
		
		for(int i=0; i<N; i++)
			_putchar(decoded_seq[i]);
	}
	
	return 0;

}

//********************************************
// Convert unipolar to bipolar 
void uni2bi(char* uni_seq, int N){
	
	for(int i=0; i<N; i++){
		if(uni_seq[i] == 0)
			uni_seq[i] = -1;
	}
}



//********************************************
/*
% MAP decoding that produces soft output and its extrinsic probability
% to be used in the next iteration of turbo code.
%
% Algorithm based on article "Maximum a Posteriori Decoding of Turbo Codes"
% by Bernard Sklar
%
*/ 
void map_decode(char* seq, float* apriori_b1, float noise_var, size_t N, float* LLR){
	
	// trellis description of encoder
	trellis trel;
	
	trel.numStates = 4;
	
	trel.nextStates[0][0] = 0; trel.nextStates[0][1] = 2;
	trel.nextStates[1][0] = 0; trel.nextStates[1][1] = 2;
	trel.nextStates[2][0] = 1; trel.nextStates[2][1] = 3;
	trel.nextStates[3][0] = 1; trel.nextStates[3][1] = 3;
	
	trel.outputs[0] = 0; trel.outputs[1] = 3;
	trel.outputs[2] = 1; trel.outputs[3] = 2;
	trel.outputs[4] = 0; trel.outputs[5] = 3;
	trel.outputs[6] = 1; trel.outputs[7] = 2;
	
	// Initiate metrics array 
	// rows are the k timestamps + 1
	// columns are the number of states or branch
	float alpha[N+1][trel.numStates];
	float beta[N+1][trel.numStates];
	float delta[N][2*trel.numStates];
	
	memset(alpha, 0, sizeof(alpha));
	memset(beta, 0, sizeof(beta));
	memset(delta, 0, sizeof(delta));
	
	// Initiate LLR array
	memset(LLR, 0, N*sizeof(float));
	
	// for columns in delta, even-indexed are branches corresponding to input 0,
	// and odd-indexed corresponds to input 1

	// Inititate alpha and beta accordingly
	// Currently assuming that start and terminating states will be always '00'
	alpha[0][0] = 1;
	beta[N][0] = 1;
	
	// Convert sequence to bipolar format
	uni2bi(seq, N*2);
	
	/////////////////////
	// Calculate Branch Metrics 'delta'
	/////////////////////
	
	for(int k=0; k<N; k++){ // iterates through N timestamps
		
		char x, y, u, v;
		float aphi;
		
		// retrieve x, y from incoming sequence
   	// ######### currently still limited to rate 1/2 code ################
   	// should be in bipolar format
   	x = seq[2*k];
   	y = seq[2*k+1];
   	
   	for(int i=0; i<(2*trel.numStates); i++){
   		
   		// apriori probability assignment, b0-> even-indexed, b1-> odd-indexed
			// following 'delta' array's indexing
    	if(i%2==0)
      	aphi = 1 - apriori_b1[k];
    	else
      	aphi = apriori_b1[k];
      	
      // determine u,v -> code prototype
      // * remember that bipolar format is necessary
			u = (trel.outputs[i] & (2))>>1;
			v = trel.outputs[i] & (1);
    	uni2bi(&u, 1);
    	uni2bi(&v, 1);
    		
			delta[k][i] = aphi*exp((x*u + y*v)/noise_var);
      //delta[k][i] = aphi*pow(euler_const, (x*u + y*v)/noise_var);
   	}
   	
	}
	
	/////////////////////
	// Calculate Branch Metrics 'alpha'
	/////////////////////
	
	for(int k=0; k<N; k++){ // iterates through N timestamps
		
		for(int i=0; i<trel.numStates; i++){
			
			// currState = i;
   		char prevStates[2] = {0};	// for storing previous states
    	char prevBranches[2] = {0}; // branches corresponding to previous states,
                               			// might be branches for bit 0 or 1
    	int idx = 1;
    	
    	// finding previous states and branches from trellis diagram
    	for(int row=0; row<trel.numStates; row++){
      	for(int col=0; col<2; col++){
        	if(trel.nextStates[row][col]==i){
          	prevStates[idx] = row;
          	prevBranches[idx] = col;
          	idx++;
        	}
      	}
    	}
    	
    	alpha[k+1][i] = delta[k][2*prevStates[0]+prevBranches[0]] * alpha[k][prevStates[0]]
                      +delta[k][2*prevStates[1]+prevBranches[1]] * alpha[k][prevStates[1]];
    	
		}
		
	}
	
	
	/////////////////////
	// Calculate Branch Metrics 'beta'
	/////////////////////
	
	for(int k=N-1; k>=0; k--){ // iterates through N timestamps
		
		for(int i=0; i<trel.numStates; i++){
			// currState = i;
    
    	// lookup for next states from the trellis diagram
    	char nextStates[2] = {trel.nextStates[i][0], trel.nextStates[i][1]};
    		
    	beta[k][i] = delta[k][2*i] * beta[k+1][nextStates[0]]
                   +delta[k][2*i+1] * beta[k+1][nextStates[1]];
		}
	
	}
	
	
	/////////////////////
	// Calculate the log likelihood ratio
	/////////////////////
	
	for(int k=0; k<N; k++){
		
		float LLR_num = 0;
  	float LLR_denom = 0;
  
  	for(int i=0; i<trel.numStates; i++){
    	// nominator part - bit 1
    	char nextState = trel.nextStates[i][1];
    	LLR_num = LLR_num + ( alpha[k][i] * delta[k][2*i+1] * beta[k+1][nextState] );
    
    	// denumerator - bit 0
    	nextState = trel.nextStates[i][0];
    	LLR_denom = LLR_denom + ( alpha[k][i] * delta[k][2*i] * beta[k+1][nextState] );
  	}
  
  	// LLR for each timestamp
  	if(LLR_num==0)
  		LLR[k] = -1;
  	else
  		LLR[k] = log10( LLR_num / LLR_denom );	
		
	}
}



