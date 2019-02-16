//------------------------------------------------------------------------------
//
// Main Program
// Application    : turboimpl_nc0
// Core           : Normal Core 0
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > calc a & b

// ### Interfacing with LMS6002D, TX part ###

#include "main.h"
#include "turbo_rumps_c1.h"

#define TX_SETUP 0xa0
#define TX_INITLIME 0xa1
#define TX_IDLE 0xa2
#define TX_ENCODE 0xa3
#define TX_ON 0xa4

void main_scheduler (void);

const unsigned char ledpin = 0;

//------------------------------------------------------------------------------
int main(void)
{
  set_trellis();
  
  // MUX - select TM_COM0 & TM_COM1
  MUXC_SELECT = 0x4;

  //Timer Settings - Lime interfacing
  TM_PR = 0x7c; // prescale target, 64kHz 
  TM_COMR0 = 0x0; //timer target, ch 0
  TM_COMCR = 0x3; // reset on COMR, enable interrupt
  TM_OCR = 0x6; // toggle TM_COM0/COM1 on match

  TM_CTRL = 0x3; // use PCLK, reset timer, enable timer
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
      digitalWrite_high(ledpin);
    }

    else if(tx_state == TX_INITLIME){
      digitalWrite_high(ledpin);
    }

    else if(tx_state == TX_IDLE){
      digitalWrite_high(ledpin);
    }

    else if(tx_state == TX_ENCODE){
      digitalWrite_low(ledpin);
    }

    else if(tx_state == TX_ON){
      digitalWrite_low(ledpin);
    }

  } 
   
  return 0;
}
  
