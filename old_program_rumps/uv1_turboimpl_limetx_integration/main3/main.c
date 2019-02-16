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

#define TX_SETUP 0xa0
#define TX_INITLIME 0xa1
#define TX_IDLE 0xa2
#define TX_ENCODE 0xa3
#define TX_ON 0xa4

const unsigned char ledpin = 0;
 
void main_scheduler (void);


//------------------------------------------------------------------------------

int main(void)

{
  set_trellis();
  
  // MUX - select TM_COM0 & TM_COM1
  MUXC_SELECT = 0x4;

  //Timer Settings - Lime interfacing
  TM_PR = 0xf9; // prescale target, 32kHz 
  TM_COMR0 = 0x0; //timer target, ch 0
  TM_COMCR = 0x3; // reset on COMR, enable interrupt
  TM_OCR = 0x6; // toggle TM_COM0/COM1 on match

  TM_CTRL = 0x3; // use PCLK, reset timer, enable timer

  asm("push {r0}\n"
      "movs  r0, #26\n"      // 1 cycle
      "loop: sub  r0, r0, #1\n" // 1 cycle
      "cmp r0, #0\n"         // 1 cycle
      "bne  loop\n"          // 2 if taken, 1 otherwise
      "pop {r0}\n");

  TM_CTRL = 0x1; // use PCLK, start timer, enable timer
  
  pinMode_output(ledpin);
  digitalWrite_high(ledpin);
  
  int tx_state;

  // *** Part1 - State indicator ***
  while(1){
    // wait for Lime's initialization to be finished
    while(noc_NC_rxbuff0_av!=1)__NOP();
    tx_state = NC_NOC_RX_BUFF0;

    if(tx_state == TX_SETUP){
      digitalWrite_low(ledpin);
    }

    else if(tx_state == TX_INITLIME){
      digitalWrite_high(ledpin);
    }

    else if(tx_state == TX_IDLE){
      digitalWrite_low(ledpin);
    }

    else if(tx_state == TX_ENCODE){
      digitalWrite_high(ledpin);
    }

    else if(tx_state == TX_ON){
      digitalWrite_low(ledpin);
    }

  }

  return 0;

}

  

