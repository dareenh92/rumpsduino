//------------------------------------------------------------------------------
//
// Main Program
// Application    : turboimpl_nc1
// Core           : Normal Core 1
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > convert LLR(ratio) -> Le(probability) format

// ### Interfacing with LMS6002D, RX part ###

#include "main.h"
#include "turbo_rumps_c2.h"

// packet structure
#define TIMINGPILOT_LEN 16
#define CHUNK_LEN 48
#define CHUNK_NUM 16
#define N_FREQPILOTPAIR 16 // pairs
#define CODEWORD_LEN 768
#define RF_PAYLOADLEN (N_FREQPILOTPAIR<<1) + CODEWORD_LEN

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

  pinMode_output(ledpin);
  digitalWrite_high(ledpin);

  // RX variable
  int qrecvBuffer[RF_PAYLOADLEN] = {0};
  unsigned int nRecv = 0;

  // Turbo decoding variable
  accum_int_t temp_accumint; //temp variable for accum & int bits manipulations
  accum ratio;
  signed char calcLLR = -1; // -1 means yet to be informed by DSP core
  
  short sent_i = 0;
  
  // *** Init Loop - Lime's initialization part ***
  while(1){
    // wait for Lime's initialization to be finished
    while(noc_NC_rxbuff0_av!=1)__NOP();
    int tempack = NC_NOC_RX_BUFF0;
    
    while(noc_NC_txbuff_isfull==1)__NOP();
    NC_NOC_TX_BUFF0 = tempack;

    break;
  }
  
  // Main Loop - sequential process
  while(1){ 
    digitalWrite_low(ledpin);
    
    // *** RX loop - buffer Q from IO core***
    for(int i=0; i<RF_PAYLOADLEN; i++){
      while(noc_NC_rxbuff0_av!=1)__NOP();
      qrecvBuffer[i] = NC_NOC_RX_BUFF0;
    }

    // *** Freq correction with DSP core ***
    unsigned int nSent = 0;
    for(int i=0; i<CHUNK_NUM; i++){
      // * send freq pilot pair
      while(noc_NC_txbuff_isfull==1)__NOP();
      NC_NOC_TX_BUFF3 = qrecvBuffer[nSent++];
      NC_NOC_TX_BUFF3 = qrecvBuffer[nSent++];
      
      for(int j=0; j<CHUNK_LEN; j++){
        while(noc_NC_txbuff_isfull==1)__NOP(); // send
        NC_NOC_TX_BUFF3 = qrecvBuffer[nSent++];

        while(noc_NC_rxbuff3_av!=1)__NOP(); // (flow control)
        int temp = NC_NOC_RX_BUFF3;
      }
    }
        
    digitalWrite_high(ledpin);
    
    // *** Turbo Decoding Part ***
    while(1){
      // receive calcLLR control flag
      if( (calcLLR==-1) && (noc_NC_rxbuff3_av==1) ){
        calcLLR = NC_NOC_RX_BUFF3;
        NC_NOC_TX_BUFF3 = calcLLR;
      }
      // && (((NC_NOC_CSR2&0xf0000000)>>28)>=0x2)
      if( (calcLLR!=-1) && (noc_NC_rxbuff3_av==1) ){
        
        // ## RECV_RATIO_DSP
        // Receive LLR format (ratio) from DSP
        temp_accumint.int_cont = NC_NOC_RX_BUFF3;
        ratio = temp_accumint.accum_cont;
        
        //change ratio -> prob for Le
        if(!calcLLR){
          ratio = exp_approx(ratio);
          ratio /= (1+ratio);
          if(ratio > 0.99990)
            ratio = 0.99990;
          else if(ratio < 0.0001)
            ratio = 0.0001;
        }
        
        // send back ACK to DSP core
        if( ((sent_i+1) & 7)==0 ){
          while(noc_NC_txbuff_isfull==1)__NOP();
          NC_NOC_TX_BUFF3 = sent_i+1;
        }

        // ## SEND_CONVERTED_IO
        // Send converted/non-converted LLR/Le to IO core
        while(noc_NC_txbuff_isfull==1)__NOP();
        temp_accumint.accum_cont = ratio;
        NC_NOC_TX_BUFF0 = temp_accumint.int_cont;
              
        // wait for ACK from IO core
        while(noc_NC_rxbuff0_av!=1)__NOP();
        int temp_ack = NC_NOC_RX_BUFF0;
        
        sent_i++;
        
        // reset flag & counter
        if(sent_i==256){
          int prevLLR = calcLLR;
          calcLLR = -1;
          sent_i = 0;

          if(prevLLR==1)
            break;
        }
      }
    } // end of decoding loop
    
  }
  
  return 0;
}
  
