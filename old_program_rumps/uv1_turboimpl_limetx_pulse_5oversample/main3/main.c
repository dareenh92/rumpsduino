//------------------------------------------------------------------------------

//

// Main Program
// Application    : turboimpl_dspc
// Core           : DSP Core
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > calc d, LLR

// ### Interfacing with LMS6002D, TX part ###

#include "main.h"
#include "turbo_rumps_c3.h"
//#include "libdivide_rumps.h"

#define IO_CHNLCTRL_HDR 0x1
#define IO_BITS_HDR 0x2
#define IO_LLRACK_HDR 0x3
#define IO_NOVAR_HDR 0x4
#define IO_STARTTURBO_HDR 0xa
#define IO_RDY 0xb
#define IO_TX 0xc

#define DSP_LLR_HDR 0x31
#define DSP_BITSACK_HDR 0x32


const unsigned char ledpin = 0;


void main_scheduler (void);


//------------------------------------------------------------------------------

int main(void)

{
  set_trellis();
  
  // MUX - select TM_COM0 & TM_COM1
  MUXC_SELECT = 0x4;

  //Timer Settings - Lime interfacing
  TM_PR = 0xc7; // prescale target, 40kHz 
  TM_COMR0 = 0x0; //timer target, ch 0
  TM_COMCR = 0x3; // reset on COMR, enable interrupt
  TM_OCR = 0x6; // toggle TM_COM0/COM1 on match

  TM_CTRL = 0x3; // use PCLK, reset timer, enable timer

  asm("push {r0}\n"
      "movs  r0, #21\n"      // 1 cycle
      "loop: sub  r0, r0, #1\n" // 1 cycle
      "cmp r0, #0\n"         // 1 cycle
      "bne  loop\n"          // 2 if taken, 1 otherwise
      "pop {r0}\n");

  TM_CTRL = 0x1; // use PCLK, start timer, enable timer
  
  pinMode_output(ledpin);
  digitalWrite_high(ledpin);
  
  // *** Init Loop - Lime's initialization part ***
  while(1){
    // wait for Lime's initialization to be finished
    while(noc_NC_rxbuff0_av!=1)__NOP();
    int tempack = NC_NOC_RX_BUFF0;
    
    while(noc_NC_txbuff_isfull==1)__NOP();
    NC_NOC_TX_BUFF0 = tempack;

    break;
  }

  
  // *** Main Loop - Turbo coding part ***
  while(1){
    __NOP();
  } 

  return 0;

}

  

