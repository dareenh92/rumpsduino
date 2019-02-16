//------------------------------------------------------------------------------

//

// Main Program
// Application    : freqsync_test_ioc
// Core           : IO Core
// Purpose
//  - Sine LUT test 
//  - Freq sync test
//  - Both above are using Accum and RUMPS' MAC 

// ###  ###

#include "main.h"
#include "fixpoint_math.h"

//------------------------------------------------------------------------------

int main(void)

{
  int lastResult;
  int currResult;
  int uartSendCount;

  uart_hd_init_uart(1); // UART, 115200 baud

  // *** Sine LUT test part ***
  
  while(1){
    // wait for command from PC
    while(uart_hd_getchar()!='s')
      __NOP();

    // sine LUT calc with DSP core
    lastResult = -1;
    uartSendCount = 0;

    for(int i=0; i<205877; i++){
      // send i - integer rep of angle (Accum type)
      while(noc_IC_txbuff_isfull==1)__NOP();
      IC_NOC_TX_BUFF3 = i;

      // wait for LUT result
      while(noc_IC_rxbuff3_av!=1)__NOP();
      currResult = IC_NOC_RX_BUFF3;

      // send to UART for every distinct LUT result
      if(currResult!=lastResult && uartSendCount++<256){
        lastResult = currResult;

        for(int j=0; j<4; j++) // send
          uart_hd_putchar( (currResult >> (j<<3)) & 0xff );

        uart_hd_getchar(); // simple ACK - flow control
      }
    }
  }
  
  // *** END - Sine LUT test ***


  // *** Freq sync test part ***
  /*
  while(1){

  }
  */
  // *** END - Freq sync test ***


  return 0;

}