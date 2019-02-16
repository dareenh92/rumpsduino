//------------------------------------------------------------------------------
//
// Test for Systick, interrupt and SEV
//
// Change priority level of Systick to 0 (highest)
// Change priority level of IRQ_0 to 3 (lowest)

#include "main.h"

volatile unsigned int plain_distributed = 0;
volatile unsigned int plain_recv_count = 0;
volatile unsigned int cipher_send_count = 0;
volatile unsigned int cipher_word_send_count = 0;

volatile unsigned int pp_data_ready = 0;
volatile unsigned int receive_noc   = 0;

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
	
	unsigned int sleep_count = 0;
	
	NVIC_SetPriority(2, 0);		// set Ext INT 2 (pp rx) priority
  NVIC_SetPriority(4, 0);   // set Ext INT 4 (noc rx) priority 
  
	NVIC_EnableIRQ(2);			  // enable Ext INT 2 (pp rx) 
  NVIC_EnableIRQ(4);        // enable Ext INT 4 (noc rx) 
	
  MUXC_SELECT = 0x2f; // set mux controller to enable parallel port IO
  
  IC_NOC_CSR2 = 0x0000f44f; // set NOC RX interrupt to be generated after receive 4 flits
  IC_NOC_CSR1 = 0x14000000; // set NOC RX interrupt and set RX IM
  
  PP_ST = 0x00000600;   // SET PP_RX_IE and PP_RX_IM
  
  main_scheduler();
   
	return 0;
}
  

void main_scheduler(void)
{
  while(1)
  {
    // -------------------------------------------------------------------------------------
    // check availability of pp data
    // -------------------------------------------------------------------------------------
    if (pp_data_ready == 1)
    {
      if (plain_recv_count % 2 == 0) {
        IC_NOC_TX_BUFF1 = PP_BUFF;
        IC_NOC_TX_BUFF1 = PP_BUFF;
        IC_NOC_TX_BUFF1 = PP_BUFF;
        IC_NOC_TX_BUFF1 = PP_BUFF;
      }
      else {
        IC_NOC_TX_BUFF2 = PP_BUFF;
        IC_NOC_TX_BUFF2 = PP_BUFF;
        IC_NOC_TX_BUFF2 = PP_BUFF;
        IC_NOC_TX_BUFF2 = PP_BUFF;
      }
      plain_recv_count++; 
      pp_data_ready = 0;
      
      NVIC_ClearPendingIRQ(2);
      
      NVIC_EnableIRQ(2);     // enable PP_RX interrupt
    }
    
    if (receive_noc == 1) {
      if (cipher_send_count % 2 == 0) {
        PP_BUFF = IC_NOC_RX_BUFF1;
        PP_BUFF = IC_NOC_RX_BUFF1;
        PP_BUFF = IC_NOC_RX_BUFF1;
        PP_BUFF = IC_NOC_RX_BUFF1;
      }
      
      else {
        PP_BUFF = IC_NOC_RX_BUFF2;
        PP_BUFF = IC_NOC_RX_BUFF2;
        PP_BUFF = IC_NOC_RX_BUFF2;
        PP_BUFF = IC_NOC_RX_BUFF2;
      }
      
      cipher_send_count++;
      receive_noc = 0;
      
      NVIC_ClearPendingIRQ(4);
      
      NVIC_EnableIRQ(4);     // enable NOC RX interrupt
      
    }
  }
}