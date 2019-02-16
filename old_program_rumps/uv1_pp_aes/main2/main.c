//------------------------------------------------------------------------------
//
// Test for Systick, interrupt and SEV
//
// Change priority level of Systick to 0 (highest)
// Change priority level of IRQ_0 to 3 (lowest)

#include "main.h"

volatile unsigned int plain_word_recv_count = 0;
volatile unsigned int cipher_send_count = 0;

volatile unsigned int receive_noc   = 0;

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
  int i = 0;
  
  unsigned int sleep_count = 0;
  
  NVIC_SetPriority(2, 0);   // set Ext INT 2 (noc rx) priority 
  //NVIC_SetPriority(3, 0);   // set Ext INT 2 (aes) priority 
  
  NVIC_EnableIRQ(2);        // enable Ext INT 2 (noc rx) 
  //NVIC_EnableIRQ(3);        // enable Ext INT 2 (aes) 
  
  IC_NOC_CSR2 = 0x0000fff4; // set NOC RX interrupt to be generated after receive 4 flits
  NC_NOC_CSR1 = 0x14000000; // set NOC RX interrupt and set RX IM
  
  //AES_CSR = 0x1;  // enable the AES interrupt
  
  main_scheduler();
   
  return 0;
}
  

void main_scheduler(void)
{
  int result;

  while(1)
  {
    if (receive_noc == 1) {
      AES_PLAIN0 = NC_NOC_RX_BUFF0;
      AES_PLAIN1 = NC_NOC_RX_BUFF0;
      AES_PLAIN2 = NC_NOC_RX_BUFF0;
      AES_PLAIN3 = NC_NOC_RX_BUFF0;
      
      receive_noc = 0;
      
      NVIC_ClearPendingIRQ(2);
      
      NVIC_EnableIRQ(2);     // enable NOC RX interrupt
    }
      
    if ((AES_CSR & 0x100) == 0x100){
      NC_NOC_TX_BUFF0 =  AES_CIPHER0;
      NC_NOC_TX_BUFF0 =  AES_CIPHER1;
      NC_NOC_TX_BUFF0 =  AES_CIPHER2;
      NC_NOC_TX_BUFF0 =  AES_CIPHER3;

      cipher_send_count++;
    }
  }
}