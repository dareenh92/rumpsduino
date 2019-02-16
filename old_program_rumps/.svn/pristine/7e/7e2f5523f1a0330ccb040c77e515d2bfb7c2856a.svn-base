//------------------------------------------------------------------------------
//
// Main Program
// Application		: timerblink
// Core						: Any
// Purpose
//	- blink an LED using timer as delay

#include "main.h"
char timer_flag = 0; 

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
	NVIC_SetPriority(6, 0);		// set Ext INT 6 (timer) priority
  
	NVIC_EnableIRQ(6);			  // enable Ext INT 6 (timer) 
	
	
	//Timer Settings
	TM_PR = 0x400; // prescale counter target value
	TM_COMR0 = 0x3D09; //timer counter target value, channel 0
	TM_COMCR = 0x3; // choose reset/stop upon reaching target value, enable interrupt
	
	TM_CTRL = 0x3; // use PCLK, reset timer, enable timer
	TM_CTRL = 0x1; // use PCLK, start timer, enable timer

  GPIO_OEN_SET = 0xffff;	// set all output
  
  while(1)
  {
	if(timer_flag==0)
		GPIO_DATAOUT = 0x0000;
	else
		GPIO_DATAOUT = 0x0001;

  }
   
	return 0;
}
  

