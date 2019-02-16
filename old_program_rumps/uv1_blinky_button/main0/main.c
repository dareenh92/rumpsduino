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
  
  	GPIO_OEN_SET = 0x000040000;	// set LED output
  
  	GPIO_OEN_CLR = 0x000080000;	// set Button input
  
  	MUXC_PU = 0xfff7ffff;	// turn off PU for Button input
  
  	GPIO_DATAOUT = 0x00000000;
  
  	while(1)
  	{
		for (j = 0; j < 20; j++)
		{
			GPIO_DATAOUT = 0x00000000;

			for (i = 0; i < 100000; i++)
	  			__NOP();

			GPIO_DATAOUT = 0x00040000;

			for (i = 0; i < 100000; i++)
	  			__NOP();
		}

		while ((GPIO_DATAIN & 0x00080000) == 0x00080000);
  	}
   
	return 0;
}
  

