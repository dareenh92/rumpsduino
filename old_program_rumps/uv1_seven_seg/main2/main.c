//------------------------------------------------------------------------------
//
// Main Program
// Application		: blinky + button
// Core						: Any
// Purpose
//	- blink an LED using corresponding core

#include "main.h"

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
	int i, j;
  
  	GPIO_OEN_SET = 0x01;	// set LED output
  
  	GPIO_OEN_CLR = 0x04;	// set Button input
  
  	MUXC_PU = 0xfb;	// turn off PU for Button input
  
  	GPIO_DATAOUT = 0x00;
  
  	while(1)
  	{
		for (j = 0; j < 5; j++)
		{
			GPIO_DATAOUT = 0x00;

			for (i = 0; i < 700000; i++)
	  			__NOP();

			GPIO_DATAOUT = 0x01;

			for (i = 0; i < 700000; i++)
	  			__NOP();
		}

		while ((GPIO_DATAIN & 0x04) == 0x04);
  	}
   
  	return 0;
}
  
