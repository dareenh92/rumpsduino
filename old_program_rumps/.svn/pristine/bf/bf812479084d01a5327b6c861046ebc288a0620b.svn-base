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
	int i = 0, j = 0;
	int a,b,c;
	int debug_event = 175;
	
	NVIC_SetPriority(0, 0);		// set Ext INT 0 priority 	
	NVIC_EnableIRQ(0);			// enable Ext INT 0
	
	a = 0x20000;
	b = 0;
	c = 0;
	
	DR_REG0 = 0x33333333;

	for (j = 0; j < 10000000; j++)
		__NOP();

	while(1)
	{
		a = a + 1;
		b = a + 2;
		c = a + 3;
		
		write32(0x800, c);
		
		i++;
		
		DR_REG1 = a;
		DR_REG2 = b;
		DR_REG3 = c;
		
		if (i == debug_event){
			i = 50;
			
			DR_START = 1;
		}
	}
   
	return 0;
}
  
