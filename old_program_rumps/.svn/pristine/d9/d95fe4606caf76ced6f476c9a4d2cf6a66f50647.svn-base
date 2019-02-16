//------------------------------------------------------------------------------
//
// Main Program
// Application		: read_appflash
// Core						: Special Core
// Purpose
//	- get special core to read program stored in its own 4kB flash
//  - get those program's data out to parallel port (debugging purpose)

#include "main.h"
#define FLASH_4K_READ		((*(volatile unsigned char *)	(0x00001000)))

void blink_led1(int rep);

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
	unsigned long i= 0;
	unsigned char * ptr;
	unsigned long data=0;
	
	MUXC_SELECT = 0x01; // set mux controller to parallel port 0-7
  
  GPIO_OEN_SET = 0xffffff;	// set GPIO [31:8]
	
	// loop for the size of sectors taken by program (in this case led_blink.bin)
	// check by running python script read_bin
	
	for(i=0; i<2048; i++){
		
		//read from 4kB address, per byte
		ptr = (volatile unsigned char *) (&FLASH_4K_READ + i);
		data = *ptr;
		
		//send data to Parallel Port buffer	
		while( (PP_ST & 0x2) == 0x2);
		PP_BUFF = data;

	}

  
  while(1){
		blink_led1(1);
  }

	return 0;
}
  
void blink_led1(int rep){
	int i = 0;
	int j = 0;
	
	for (i = 0; i < rep; i++){
  	GPIO_DATAOUT = 0x0100;
		for (j = 0; j < 1000000; j++)
			__NOP();
				
		GPIO_DATAOUT = 0x0000;
		for (j = 0; j < 1000000; j++)
			__NOP();
	}
}
