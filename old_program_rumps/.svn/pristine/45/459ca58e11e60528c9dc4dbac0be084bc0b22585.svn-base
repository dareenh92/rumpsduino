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
	
  //Timer Settings - toggle pin GPIO0_8 every cycle
  //NVIC_SetPriority(6, 0);		// set Ext INT 6 (timer) priority
  
	//NVIC_EnableIRQ(6);			  // enable Ext INT 6 (timer) 
	
	//PR 400, COMR0 3D08
	TM_PR = 0x1; // prescale counter target value
	TM_COMR0 = 0x0; //timer counter target value, channel 0
	TM_COMCR = 0x3; // choose reset/stop upon reaching target value, enable interrupt
	
	TM_OCR = 0x6; // use timer hardware-controlled pin to toggle
	
	TM_CTRL = 0x3; // use PCLK, reset timer, enable timer
	TM_CTRL = 0x1; // use PCLK, start timer, enable timer
  
  MUXC_SELECT = 0x11; // set mux controller to parallel port 0-7, and timer IO
  
  GPIO_OEN_SET = 0xffff00;	// set GPIO [31:8] output
  
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
			PP_BUFF = 65;
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
