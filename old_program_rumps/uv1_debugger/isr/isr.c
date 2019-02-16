//------------------------------------------------------------------------------
//

#include "main.h"

// cannot and should not use printf(), data sharing problem
// message() should be reentrant (by default) 

unsigned int systick_cnt = 0;
unsigned int ext_0_cnt = 0;
unsigned int ext_1_cnt = 0;

void NMI_Handler(void)
{
	int i;
	
	__SEV();
	
	//write32(0x200, 0x11111111);
  	//while(1);
  	for (i = 0; i < 2000; i ++);
  	__SEV();
}

void HARD_FAULT_Handler(void)
{
	write32(0x200, 0x22222222);
	while(1);
}

void SYSTICK_Handler(void)
{
	__SEV();
  	systick_cnt++;
  	write32(0x200, 0x33333333);
	while(1);
}

void EXTERNAL_0_Handler(void)
{
	int i;
	
	__SEV();
	ext_0_cnt++;
	NVIC_ClearPendingIRQ(0);
	
	for (i = 0; i < 200; i ++);
	
	__SEV();
}


void EXTERNAL_1_Handler(void)
{
	__SEV();

	NVIC_ClearPendingIRQ(1);
  	ext_1_cnt++;
}
