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

#define TIMINGPILOT_LEN 8
#define CODEWORD_LEN 768
#define RF_PKTLEN TIMINGPILOT_LEN + CODEWORD_LEN

#define IO_CHNLCTRL_HDR 0x1
#define IO_BITS_HDR 0x2
#define IO_LLRACK_HDR 0x3
#define IO_NOVAR_HDR 0x4
#define IO_STARTTURBO_HDR 0xa
#define IO_RDY 0xb
#define IO_TX 0xc
#define IO_GETPULSE 0xd

#define DSP_LLR_HDR 0x31
#define DSP_BITSACK_HDR 0x32

#define RCOS_FIRLEN 12

const unsigned char ledpin = 0;

//**for pulse shaping
unsigned char sendPkt[RF_PKTLEN];
int rcos_lut[2][RCOS_FIRLEN]; // raised cosine impulse response LUT
int arrctr0, arrctr1, arrctr2, arrctr3, arrctr4; // pulse components counter
unsigned int symctr0, symctr1, symctr2, symctr3, symctr4; // symbol counter
volatile int send_buff = 0;
void init_pulseshaping();

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
      "movs  r0, #16\n"      // 1 cycle
      "loop: sub  r0, r0, #1\n" // 1 cycle
      "cmp r0, #0\n"         // 1 cycle
      "bne  loop\n"          // 2 if taken, 1 otherwise
      "pop {r0}\n");

  TM_CTRL = 0x1; // use PCLK, start timer, enable timer
  
  pinMode_output(ledpin);
  digitalWrite_high(ledpin);
  
  // initialize pulse shaping function variables
  init_pulseshaping();

  // Define variables
  int pkt_type = 0;

  // *** Init Loop - Lime's initialization part ***
  while(1){
    // wait for Lime's initialization to be finished
    while(noc_NC_rxbuff0_av!=1)__NOP();
    int tempack = NC_NOC_RX_BUFF0;
    
    while(noc_NC_txbuff_isfull==1)__NOP();
    NC_NOC_TX_BUFF0 = tempack;

    break;
  }

  // receive packet to be pulse-shaped
  for(int i=0; i<RF_PKTLEN; i++){
    while(noc_NC_rxbuff0_av!=1)__NOP();
    sendPkt[i] = NC_NOC_RX_BUFF0;

    while(noc_NC_txbuff_isfull==1)__NOP();
    NC_NOC_TX_BUFF0 = sendPkt[i];
  }

  digitalWrite_low(ledpin);

  // *** Main Loop - Turbo coding part ***
  while(1){
    // calculate pulse value
    send_buff = 0;

    if((arrctr0>=0) && (symctr0<RF_PKTLEN))
      send_buff += rcos_lut[sendPkt[symctr0]][arrctr0];

    if((arrctr1>=0) && (symctr1<RF_PKTLEN))
      send_buff += rcos_lut[sendPkt[symctr1]][arrctr1];

    if((arrctr2>=0) && (symctr2<RF_PKTLEN))
      send_buff += rcos_lut[sendPkt[symctr2]][arrctr2];

    if(++arrctr0 == RCOS_FIRLEN){
      arrctr0 = 0;
      symctr0 += 3;
    }

    if(++arrctr1 == RCOS_FIRLEN){
      arrctr1 = 0;
      symctr1 +=3;
    }

    if(++arrctr2 == RCOS_FIRLEN){
      arrctr2 = 0;
      symctr2 += 3;
    }
    
    if(symctr0>=RF_PKTLEN && symctr1>=RF_PKTLEN && symctr2>=RF_PKTLEN){ 
      arrctr0 = 0;
      arrctr1 = -4;
      arrctr2 = -8;

      symctr0 = 0;
      symctr1 = 1;
      symctr2 = 2;
    }
    
    // send pulse value
    while(noc_NC_rxbuff0_av!=1)__NOP();
    pkt_type = NC_NOC_RX_BUFF0;
    while(noc_NC_txbuff_isfull==1)__NOP();
    NC_NOC_TX_BUFF0 = send_buff;
    
  } 

  return 0;

}

void init_pulseshaping(){
  // set LUT
  // ** bit 0
  rcos_lut[0][0] = rcos_lut[0][8] = 0;
  rcos_lut[0][1] = rcos_lut[0][7] = -269;
  rcos_lut[0][2] = rcos_lut[0][6] = -615;
  rcos_lut[0][3] = rcos_lut[0][5] = -909;
  rcos_lut[0][4] = -1024;

  // ** bit 1
  rcos_lut[1][0] = rcos_lut[1][8] = 0;
  rcos_lut[1][1] = rcos_lut[1][7] = 269;
  rcos_lut[1][2] = rcos_lut[1][6] = 615;
  rcos_lut[1][3] = rcos_lut[1][5] = 909;
  rcos_lut[1][4] = 1024;

  // dummy guard band
  rcos_lut[0][9] = rcos_lut[0][10] = rcos_lut[0][11] = 0;
  rcos_lut[1][9] = rcos_lut[1][10] = rcos_lut[1][11] = 0;

  // init counters
  arrctr0 = 0;
  arrctr1 = -4;
  arrctr2 = -8;

  symctr0 = 0;
  symctr1 = 1;
  symctr2 = 2;
}