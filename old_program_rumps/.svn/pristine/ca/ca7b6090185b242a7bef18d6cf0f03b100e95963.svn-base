//------------------------------------------------------------------------------
//
// Main Program
// Application		: turboimpl_nc0_demo2
// Core						: Normal Core 0
// Purpose
//	- Turbo code implementation on RUMPS401
//  - > calc b

#include "main.h"
#include "turbo_rumps_c1.h"

const unsigned char ledpin = 0;

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
  set_trellis();
  pinMode_output(ledpin);
  
  accum local_deltabuff[r_win][16];
  accum local_betabuff[r_win][8];
  
  accum_int_t temp_accumint; //temp variable for manipulating bits between accum and int type
  
  // Init beta
  for(char i=0; i<8; i++)
		local_betabuff[r_win-1][i] = -2.0794;
  
  while(1){
  	// ## RECV_DELTA_DSP
  	// Receive and collect delta for full window size
    for(char i=0; i<r_win; i++){
      for(char j=0; j<16; j++){
        while((NC_NOC_CSR0 & 0x40)!=0x40)__NOP();
        temp_accumint.int_cont = NC_NOC_RX_BUFF3;
        local_deltabuff[i][j] = temp_accumint.accum_cont;
      }
    }
  	
  	// ## CALC_SEND_BETA_DSP
		// Calculate and send beta to DSP core	
		for(signed char i=r_win-2; i>=0; i--){
			r_turbodec_bcalc(local_deltabuff[i+1], local_betabuff[i+1], local_betabuff[i]);
			
		  for(char j=0; j<8; j++){
		    while((NC_NOC_CSR1 & 0x2)==0x2)__NOP();
		    temp_accumint.accum_cont = local_betabuff[i][j]; 
		    NC_NOC_TX_BUFF3 = temp_accumint.int_cont;
		  }
		  //wait for ACK
		  while((NC_NOC_CSR0 & 0x40)!=0x40)__NOP();
		  int temp_ack = NC_NOC_RX_BUFF3;
		}
		
		//blink led
	  GPIO_BTGL = 0x1 << ledpin;	
  }	
   
	return 0;
}
  
