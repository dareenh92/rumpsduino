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
	int i = 0;
  
  unsigned int sleep_count = 0;
  
  NVIC_SetPriority(2, 0);   // set Ext INT 0 priority   
  NVIC_EnableIRQ(2);      // enable Ext INT 0
  
  NC_NOC_CSR1 = 0x04000000; // set NOC RX interrupt
  
  while(1)
  {
    __WFI();
  }
   
  return 0;
}
  
