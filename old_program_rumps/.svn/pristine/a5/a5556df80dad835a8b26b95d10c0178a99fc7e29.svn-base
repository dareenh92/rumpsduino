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
	
	for (i = 0; i < 10000; i++){
    __NOP();
  }
  
	while(1)
	{
	  DR_REG0 = 0x11110000+sleep_count;
    DR_REG1 = 0x11120000+sleep_count;
    DR_REG2 = 0x11130000+sleep_count;
    DR_REG3 = 0x11140000+sleep_count;
  
    if (sleep_count % 4 == 0) {
      for (i = 0; i < 100; i++){
        __NOP();
      }
    
      DR_START = 1;
    }
	
		__WFI();
		
		sleep_count++;
	}
   
	return 0;
}
  
