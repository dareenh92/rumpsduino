//------------------------------------------------------------------------------
//
// Main Program
// Application		: pp_test
// Core						: Special Core
// Purpose
//	- testing parallel port
//	- receive and send back data through parallel port

#include "main.h"

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
  volatile int i=0;
  int k=0;
  volatile short flag=0;
	
  volatile unsigned char core_byte[64];

  
  MUXC_SELECT = 0x01; // set mux controller to parallel port 0-7
  
  GPIO_OEN_SET = 0xffff00;	// set GPIO [31:8] output
  
  volatile unsigned char data_sum = 0;
  
  while(1){
  	if( (PP_ST & 0x4) == 0x4 ){ //receive
  		core_byte[i] = PP_BUFF;
  		i+=1;
  		
  		if(i%4==0)
  			flag = 1;
  	}
		
		if(flag==1){ //send ACK
			//for(k=0; k<10000; k++) __NOP();
			while( (PP_ST & 0x2) == 0x2);
			PP_BUFF = core_byte[i-4]+core_byte[i-3];
			flag = 0;
		}
		
		if(i==64){ //send back datas
			for(k=0; k<64; k++){
				while( (PP_ST & 0x2) == 0x2);
				PP_BUFF = core_byte[k];
			}
			break;
		}
		
  }
	return 0;
}
