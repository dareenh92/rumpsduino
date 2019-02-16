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
  
  GPIO_OEN_SET = 0xffff;	// set all output
  
  GPIO_DATAOUT = 0x0000;
  
  while(1)
  {
	GPIO_DATAOUT = 0x0000;

	for (i = 0; i < 700000; i++)
	  __NOP();

	GPIO_DATAOUT = 0x0001;

	for (i = 0; i < 700000; i++)
	  __NOP();
  }
   
  return 0;
}
  
