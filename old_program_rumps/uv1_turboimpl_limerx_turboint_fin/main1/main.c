//------------------------------------------------------------------------------
//
// Main Program
// Application    : turboimpl_nc0
// Core           : Normal Core 0
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > calc a & b

// ### Interfacing with LMS6002D, RX part ###

#include "main.h"
#include "turbo_rumps_c1.h"

#define RX_STOP 0xa0
#define RX_DETECT 0xa1
#define RX_TIMING_SYNC 0xa2
#define RX_FREQ_SYNC 0xa3
#define RX_PAYLOAD 0xa4
#define RX_PHASECORR 0xa5
#define RX_DECODE 0xa6
#define RX_SENDUP 0xa7
#define STATE_EMPTY 0xaf


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

  // Turbo decoding variables
  short drecv_i = 0;
  unsigned char itrcount = 0;
  
  accum local_deltabuff[r_win][16];
  accum local_alphabuff[r_win+1][8];
  accum local_betabuff[r_win][8];
  
  accum_int_t temp_accumint; // temp variable for accum & int bit manipulation

  // Init beta
  for(char i=0; i<8; i++)
    local_betabuff[r_win-1][i] = -2.0794;

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
    // ## Init alpha based on window index ##
    if(drecv_i==0){ // init for 1st window
      local_alphabuff[0][0] = 0;
      local_alphabuff[0][1] = local_alphabuff[0][2] = local_alphabuff[0][3] = -128;
      local_alphabuff[0][4] = local_alphabuff[0][5] = local_alphabuff[0][6] = local_alphabuff[0][7] = -128;
    }
    else{ //init by previous window
      for(char i=0; i<8; i++)
        local_alphabuff[0][i] = local_alphabuff[r_win][i];
    }
    
    // ## Receive and collect delta for full window size ##
    for(char i=0; i<r_win; i++){
      for(char j=0; j<16; j++){
        while(noc_NC_rxbuff3_av!=1)__NOP();
        temp_accumint.int_cont = NC_NOC_RX_BUFF3;
        local_deltabuff[i][j] = temp_accumint.accum_cont;
      }
      drecv_i++;
      drecv_i &= (255);
      // (drecv_i)==(0) ? (itrcount++) : (itrcount+=0);
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF3 = drecv_i;
    }
    
    // ## Calculate alpha ##
    for(char i=1; i<=r_win; i++){
      r_turbodec_acalc(local_deltabuff[i-1], local_alphabuff[i-1], local_alphabuff[i]);
    }
    // ## Calculate beta ##
    for(signed char i=r_win-2; i>=0; i--){
      r_turbodec_bcalc(local_deltabuff[i+1], local_betabuff[i+1], local_betabuff[i]);
    }
    
    // ## Send alpha ##
    for(char i=1; i<=r_win; i++){
      for(char j=0; j<8; j++){
        while(noc_NC_txbuff_isfull==1)__NOP();
        temp_accumint.accum_cont = local_alphabuff[i-1][j];
        NC_NOC_TX_BUFF3 = temp_accumint.int_cont;
      }
      //wait for ACK
      while(noc_NC_rxbuff3_av!=1)__NOP();
      int temp_ack = NC_NOC_RX_BUFF3;
    }
    // ## Send beta ##
    for(signed char i=r_win-2; i>=0; i--){
      for(char j=0; j<8; j++){
        while(noc_NC_txbuff_isfull==1)__NOP();
        temp_accumint.accum_cont = local_betabuff[i][j]; 
        NC_NOC_TX_BUFF3 = temp_accumint.int_cont;
      }
      //wait for ACK
      while(noc_NC_rxbuff3_av!=1)__NOP();
      int temp_ack = NC_NOC_RX_BUFF3;
    }
  }   
   
  return 0;
}
  
