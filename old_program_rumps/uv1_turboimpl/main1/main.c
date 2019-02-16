//------------------------------------------------------------------------------
//
// Main Program
// Application		: turboimpl_nc0
// Core						: Normal Core 0
// Purpose
//	- Turbo code implementation on RUMPS401
//  - > calc a & b

#include "main.h"
#include "turbo_rumps_c1.h"

const unsigned char ledpin = 0;

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
  set_trellis();
  pinMode_output(ledpin);
  
  //Timer Settings - Lime interfacing
  TM_PR = 0x1; // prescale target, 4MHz 
  TM_COMR0 = 0x0; //timer target, ch 0
  TM_COMCR = 0x3; // reset on COMR, enable interrupt
  TM_OCR = 0x6; // toggle TM_COM0/COM1 on match

  TM_CTRL = 0x3; // use PCLK, reset timer, enable timer
  TM_CTRL = 0x1; // use PCLK, start timer, enable timer
  
  // MUX - select TM_COM0 & TM_COM1
  MUXC_SELECT = 0x4;

  short drecv_i = 0;
  unsigned char itrcount = 0;
  
  accum local_deltabuff[r_win][16];
  accum local_alphabuff[r_win+1][8];
  accum local_betabuff[r_win][8];
  
  accum_int_t temp_accumint; //temp variable for manipulating bits between accum and int type
  
  // Init beta
  for(char i=0; i<8; i++)
		local_betabuff[r_win-1][i] = -2.0794;
  
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
      while((NC_NOC_CSR1 & 0x2)==0x2)__NOP(); NC_NOC_TX_BUFF3 = drecv_i;
    }
    
  	// ## CALC_ALPHA_BETA
		// Calculate alpha
		for(char i=1; i<=r_win; i++){
			r_turbodec_acalc(local_deltabuff[i-1], local_alphabuff[i-1], local_alphabuff[i]);
		}
		// Calculate beta
		for(signed char i=r_win-2; i>=0; i--){
			r_turbodec_bcalc(local_deltabuff[i+1], local_betabuff[i+1], local_betabuff[i]);
		}
		
  	// ## SEND_ALPHA_BETA_DSP
    // Send alpha
		for(char i=1; i<=r_win; i++){
		  for(char j=0; j<8; j++){
		    while((NC_NOC_CSR1 & 0x2)==0x2)__NOP();
		    temp_accumint.accum_cont = local_alphabuff[i-1][j];
		    NC_NOC_TX_BUFF3 = temp_accumint.int_cont;
		  }
      //wait for ACK
      while((NC_NOC_CSR0 & 0x40)!=0x40)__NOP();
      int temp_ack = NC_NOC_RX_BUFF3;
      //debug - write i count to IO core
      //while((NC_NOC_CSR1 & 0x2)==0x2)__NOP(); NC_NOC_TX_BUFF0 = temp_ack;
      //while((NC_NOC_CSR0 & 0x1)!=0x1)__NOP(); temp_ack = NC_NOC_RX_BUFF0;
		}
    //Send beta
		for(signed char i=r_win-2; i>=0; i--){
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
  
