//------------------------------------------------------------------------------

//

// Main Program
// Application    : turboimpl_dspc
// Core           : DSP Core
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > calc d, LLR

// ### Interfacing with LMS6002D, RX part ###

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

#define RX_STOP 0xa0
#define RX_DETECT 0xa1
#define RX_TIMING_SYNC 0xa2
#define RX_FREQ_SYNC 0xa3
#define RX_PAYLOAD 0xa4
#define RX_SENDUP 0xa5

const unsigned char ledpin = 0;


void main_scheduler (void);


//------------------------------------------------------------------------------

int main(void)

{
  set_trellis();
  
  // MUX - select TM_COM0 & TM_COM1
  MUXC_SELECT = 0x4;

  pinMode_output(ledpin);
  digitalWrite_high(ledpin);
  
  // Define variables
  int Idata, Qdata;
  uint8_t rx_state = RX_STOP;
  
  // * use these for TED-sign version
  int Isamp[3]; // 0-prev, 1-mid, 2-curr
  int Qsamp[3];
  
  // * use these for TED-value version
  // accum_int_t Isamp[3]; // 0-prev, 1-mid, 2-curr
  // accum_int_t Qsamp[3];
  
  unsigned int nSamples = 0;
  accum ted_total;
  int tempcalc = 0;
  int ted = 0;
  int stepCorrection = 0;

  accum_int_t temp_accumint;

  // *** Init Loop - Lime's initialization part ***
  while(1){
    // wait for Lime's initialization to be finished
    while(noc_NC_rxbuff0_av!=1)__NOP();
    int tempack = NC_NOC_RX_BUFF0;
    
    while(noc_NC_txbuff_isfull==1)__NOP();
    NC_NOC_TX_BUFF0 = tempack;

    break;
  }
  
  // *** Main Loop - RX Sync part ***
  while(1){

    //determine sync state 
    while(noc_NC_rxbuff0_av!=1)__NOP();  
    rx_state = NC_NOC_RX_BUFF0;

    // DETECT - DSP core doesnt take part
    
    // TIMING_SYNC - TED algorithm
    if(rx_state==RX_TIMING_SYNC){
      
      //receive I and Q
      while(noc_NC_rxbuff0_av!=1)__NOP(); Idata = NC_NOC_RX_BUFF0;
      while(noc_NC_rxbuff0_av!=1)__NOP(); Qdata = NC_NOC_RX_BUFF0;

      Isamp[nSamples] = Idata;
      Qsamp[nSamples] = Qdata;
    
      if(++nSamples==3){
        nSamples = 0;

        // TED calc - Sign version 
        // I-part
        tempcalc = sign_f(Isamp[2])- sign_f(Isamp[0]);
        tempcalc = mac_smul_32((int32_t)(sign_f(Isamp[1])),
                               (int32_t)(tempcalc));
        ted += tempcalc;

        // Q-part
        tempcalc = sign_f(Qsamp[2])- sign_f(Qsamp[0]);
        tempcalc = mac_smul_32((int32_t)(sign_f(Qsamp[1])),
                               (int32_t)(tempcalc));
        ted += tempcalc;

        // determine step correction
        stepCorrection = sign_f(ted);

        //send back synchronization result
        while(noc_NC_txbuff_isfull==1)__NOP();
        NC_NOC_TX_BUFF0 = stepCorrection;
      }
    }

    // FREQ_SYNC - calc phase offset 
    if(rx_state==RX_FREQ_SYNC){

    }
    
    // PAYLOAD - correct phase offset
    if(rx_state==RX_PAYLOAD){
      
    }


  } 

  return 0;

}