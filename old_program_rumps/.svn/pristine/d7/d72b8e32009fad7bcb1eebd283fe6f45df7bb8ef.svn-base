//------------------------------------------------------------------------------
//
// Test for Systick, interrupt and SEV
//
// Change priority level of Systick to 0 (highest)
// Change priority level of IRQ_0 to 3 (lowest)

#include "main.h"

//------------------------------------------------------------------------------
void blink_led();

//------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
  
	//****************
	// Configuration phase
	//****************
	// Select IO Muxing
	MUXC_SELECT = 0x0; // all GPIO
	MUXC_PU &= 0x7f; // turn off PU

	// Set gpio pins mode
	GPIO_OEN_SET = 0x10;	// set GPIO_4 output
	GPIO_BCLR = 0x10;
	
	// Set gpio pins mode
	GPIO_OEN_SET = 0x80;	// set GPIO_7 output
	GPIO_BCLR = 0x80;
  
  //Enable processor's interrupt
	//* Ext INT 2 - NoC RX
  NVIC_SetPriority(2, 0);   
  NVIC_EnableIRQ(2); 
	
  NC_NOC_CSR1 = 0x04000000; // set NOC RX interrupt
    
 	//****************
	// Main loop
	//****************
  while(1){
		__NOP();
		
		__WFI();
			
		blink_led();
	
  	IC_NOC_TX_BUFF0 = 0xf;
	}
  
  //__WFI();
  //while(1) __NOP();
   
  return 0;
}
  
void blink_led(){
	int i;
	
	// turn on LED
	GPIO_BCLR = 0x10;
	
	// some delays
  for (i = 0; i < 700000; i++)
    __NOP();
  
  // turn off LED
  GPIO_BSET = 0x10;
}
