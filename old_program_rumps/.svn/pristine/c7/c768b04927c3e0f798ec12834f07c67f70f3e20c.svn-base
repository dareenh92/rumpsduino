//------------------------------------------------------------------------------
//
// Test for Systick, interrupt and SEV
//
// Change priority level of Systick to 0 (highest)
// Change priority level of IRQ_0 to 3 (lowest)

#include "main.h"

//------------------------------------------------------------------------------
int main(void)
{

  //PR 400, COMR0 3D08
  TM_PR = 0x0; // prescale counter target value
  TM_COMR0 = 0x0; //timer counter target value, channel 0
  TM_COMCR = 0x3; // choose reset/stop upon reaching target value, enable interrupt

  TM_OCR = 0x6; // use timer hardware-controlled pin to toggle

  TM_CTRL = 0x3; // use PCLK, reset timer, enable timer
  TM_CTRL = 0x1; // use PCLK, start timer, enable timer

  MUXC_SELECT = 0x04; // set mux controller to COM0 timer IO


  while(1)
  {
    __NOP();
  }
   
  return 0;
}
  
