//------------------------------------------------------------------------------
//
// Main Program
// Application		: access_othercore
// Core						: Special Core
// Purpose
//	- get special core to write to other cores' flash through NoC bridge
//	- we can check the data later on using flash wrapper	

#include "main.h"

#define FLASH_4K_READ		((*(volatile unsigned char *)	(0x00001000)))
void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
	unsigned long i= 0;
	unsigned char * ptr;
	unsigned long data=0;
	
	//write data on first 2 sector of other cores' flash
	for(i=0; i<2048; i++){
		
		//read from 4kB address, per byte
		ptr = (volatile unsigned char *) (&FLASH_4K_READ + i);
		data = *ptr;
		
		//send data to Noc buffer		
			
		IC_NOC_TX_BD_BUFF1 = data;
  	//IC_NOC_TX_BD_BUFF2 = data;
 		//IC_NOC_TX_BD_BUFF3 = data;

		//wait for corresponding ACK by polling
		//while( (IC_NOC_BD_CSR & 0x8)!=0x8 && (IC_NOC_BD_CSR & 0x20)!=0x20 && (IC_NOC_BD_CSR & 0x80)!=0x80 )
		while( (IC_NOC_BD_CSR & 0x8)!=0x8 )	//0xa8
			__NOP();
			
		//reset those ACK bits
		IC_NOC_BD_CSR = 0x0000008;
		
	}

	GPIO_OEN_SET = 0xffff;	// set all output
  
  GPIO_DATAOUT = 0x0000;
  
  while(1){
		GPIO_DATAOUT = 0x0000;

		for (i = 0; i < 1000000; i++)
	  	__NOP();

		GPIO_DATAOUT = 0x0001;

		for (i = 0; i < 100000; i++)
	  	__NOP();
  }

	return 0;
}
  

