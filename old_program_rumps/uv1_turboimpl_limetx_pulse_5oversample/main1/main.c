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

#define IO_STARTTURBO_HDR 0xa
#define IO_RDY 0xb
#define IO_TX 0xc

void main_scheduler (void);

const unsigned char ledpin = 0;

//------------------------------------------------------------------------------
int main(void)
{
  set_trellis();
  
  // MUX - select TM_COM0 & TM_COM1
  MUXC_SELECT = 0x4;

  //Timer Settings - Lime interfacing
  TM_PR = 0x63; // prescale target, 80kHz 
  TM_COMR0 = 0x0; //timer target, ch 0
  TM_COMCR = 0x3; // reset on COMR, enable interrupt
  TM_OCR = 0x6; // toggle TM_COM0/COM1 on match

  TM_CTRL = 0x3; // use PCLK, reset timer, enable timer
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
  uint32_t ioc_pkt;
  while(1){
    
    while(noc_NC_rxbuff0_av!=1)__NOP();
    ioc_pkt = NC_NOC_RX_BUFF0;
    
    if(ioc_pkt==IO_RDY){
      digitalWrite_low(ledpin);
    }

    else if(ioc_pkt==IO_TX){
      digitalWrite_high(ledpin);
    }
  }   
   
  return 0;
}
  
