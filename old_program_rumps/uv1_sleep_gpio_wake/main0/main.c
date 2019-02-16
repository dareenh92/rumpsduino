//------------------------------------------------------------------------------
//
// Test for Systick, interrupt and SEV
//
// Change priority level of Systick to 0 (highest)
// Change priority level of IRQ_0 to 3 (lowest)

#include "main.h"

#define MAX_LOOP 10

volatile char i_flag = 0;

//------------------------------------------------------------------------------
void blink_led();

//------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
	
	//****************
	// Configuration phase
	//****************
	
	// Set gpio pins mode
	GPIO_OEN_SET = 0x10; // set GPIO_4 output
	GPIO_OEN_CLR = 0x1; // set GPIO_0 input
	
	//Enable processor's interrupt
	//* Ext INT 4 - NoC RX
	NVIC_SetPriority(4, 0);	
	NVIC_EnableIRQ(4);
	
	//* Ext INT 8 - GPIO0
	NVIC_SetPriority(8, 0);	
	NVIC_EnableIRQ(8);
	
  IC_NOC_CSR1 = 0x04000000; // turn on NOC RX interrupt
  
  //set GPIO interrupt registers
  GPIO_IT_CLR = 0x1; //level triggered
  GPIO_IP_CLR = 0x1; //low-level
  GPIO_IE_SET = 0x1; //GPIO_0
  
	//****************
	// Main loop
	//****************
  while(1){
		for(i=0; i<MAX_LOOP; i++){
			//while(!i_flag);
			
			blink_led();
			//i_flag = 0;
			
  		// wake NC1 through NoC
  		IC_NOC_TX_BUFF1 = 0xf;
			
			__NOP();
			__WFI();
		}
		
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
