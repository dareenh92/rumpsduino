//------------------------------------------------------------------------------
//
// Main Program
// Application		: blinky
// Core						: Any
// Purpose
//	- blink an LED using corresponding core

#include "main.h"

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
  int i = 0;
  
  pinMode_output(0);
  
  while(1){
  	digitalWrite_high(0);

		for (i = 0; i < 100000; i++)
	  	__NOP();

		digitalWrite_low(0);

		for (i = 0; i < 100000; i++)
	  	__NOP();
  }	
   
	return 0;
}
  
