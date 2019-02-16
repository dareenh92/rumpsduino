//------------------------------------------------------------------------------
//
// Main Program
// Application		: turboimpl_nc1_DEMO
// Core						: Normal Core 1
// Purpose
//	- Turbo code implementation on RUMPS401
//  - > calc a

#include "main.h"
#include "turbo_rumps_c2.h"

const unsigned char ledpin = 0;

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
  set_trellis();
  pinMode_output(ledpin);
  
  short drecv_i = 0;
  unsigned char itrcount = 0;
  
  accum local_deltabuff[r_win][16];
  accum local_alphabuff[r_win+1][8];
  
  accum_int_t temp_accumint; //temp variable for manipulating bits between accum and int type
  
  while(1){
  	// ## INIT_ALPHA
  	// Init alpha based on window index
  	if(drecv_i==0){ // init for 1st window
  	  local_alphabuff[0][0] = 0;
			local_alphabuff[0][1] = local_alphabuff[0][2] = local_alphabuff[0][3] = -128;
			local_alphabuff[0][4] = local_alphabuff[0][5] = local_alphabuff[0][6] = local_alphabuff[0][7] = -128;
  	}
  	else{ //init by previous window
  	  for(char i=0; i<8; i++)
  	    local_alphabuff[0][i] = local_alphabuff[r_win][i];
  	}
  	
  	// ## RECV_DELTA_DSP
  	// Receive and collect delta for full window size
    for(char i=0; i<r_win; i++){
      for(char j=0; j<16; j++){
        while((NC_NOC_CSR0 & 0x40)!=0x40)__NOP();
        temp_accumint.int_cont = NC_NOC_RX_BUFF3; 
        local_deltabuff[i][j] = temp_accumint.accum_cont;
      }
      drecv_i++;
      drecv_i &= (255);
      (drecv_i)==(0) ? (itrcount++) : (itrcount+=0);
    }
	  
	  // ## CALC_SEND_ALPHA_DSP
		// Calculate and send alpha to DSP core	
		for(char i=1; i<=r_win; i++){
			r_turbodec_acalc(local_deltabuff[i-1], local_alphabuff[i-1], local_alphabuff[i]);
			
		  for(char j=0; j<8; j++){
		    while((NC_NOC_CSR1 & 0x2)==0x2)__NOP();
		    temp_accumint.accum_cont = local_alphabuff[i-1][j];
		    NC_NOC_TX_BUFF3 = temp_accumint.int_cont;
		  }
		  //wait for ACK
		  while((NC_NOC_CSR0 & 0x40)!=0x40)__NOP();
		  int temp_ack = NC_NOC_RX_BUFF3;
		}
		
		//blink led
	  GPIO_BTGL = 0x1 << ledpin;
	  
	  //signal to IO core
	  if(itrcount==2){
		  while((NC_NOC_CSR1 & 0x2)==0x2)__NOP();
		  NC_NOC_TX_BUFF0 = 0x1;
	  }
  }	
   
	return 0;
}
  
