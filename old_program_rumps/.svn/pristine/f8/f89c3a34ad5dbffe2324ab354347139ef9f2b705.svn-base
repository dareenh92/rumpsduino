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
  
  	  	 // CPHA, CPOL, BC,   IE,   FSB,  SS,   CR
  spi_init( 0x1,  0x1,  0x0,  0x0,  0x0,  0x1,  0x1);

  spi_enable();

  //SPI_CTRL = 0xc4;
  //SPI_CRSEL = 0x00;

  // mux
  MUXC_SELECT = 0x00000040;	// SPI selected
  MUXC_PU &= 0x0fffffff;	// SPI PU disabled

  while(1)
  {
	for (i = 0; i < 100000; i++)
	  __NOP();

	spi_send(0xf5);

	for (i = 0; i < 100000; i++)
	  __NOP();

	spi_send(0xa0);
  }
   
  return 0;
}
  

