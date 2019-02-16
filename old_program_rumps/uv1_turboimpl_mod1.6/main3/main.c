//------------------------------------------------------------------------------

//

// Main Program
// Application    : turboimpl_dspc
// Core           : DSP Core
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > calc d, LLR ratio, convert LLR ratio->prob

#include "main.h"
#include "turbo_rumps_c3.h"

// Packet header
#define EMPTY_HDR 0x00000000

#define IO_CHNLCTRL_HDR 0x10000001
#define IO_BITS_HDR 0x10000002
#define IO_LLR_R_ACK_HDR 0x10000003
#define IO_LLR_P_ACK_HDR 0x10000004

#define NC_AB_HDR 0x20000001
#define NC_DELTA_CTS_HDR 0x20000002
#define NC_DELTA_ACK_HDR 0x20000003
#define NC_LLR_R_ACK_HDR 0x20000004

#define DSP_BITS_ACK_HDR 0x30000001
#define DSP_DELTA_HDR 0x30000002
#define DSP_AB_CTS_HDR 0x30000003
#define DSP_AB_ACK_HDR 0x30000004
#define DSP_LLR_R_HDR 0x30000005
#define DSP_LLR_P_HDR 0x30000006

// State flag
#define GEN_FLAG_DOWN 0
#define GEN_FLAG_UP 1
#define GEN_FLAG_HOLD 2

// IO pins
const uint8_t ledpin = 0;


void main_scheduler (void);


//------------------------------------------------------------------------------

int main(void)
{
  // ## Variables ##
  // counter
  int16_t iorecv_i = 0; //count bits received from IO core
  int16_t retllr_i = 0; //count returned LLR calculation
  int16_t delta_i = 0;
  int8_t delta_j = 0;
  int16_t alpha_i = 0;
  int8_t alpha_j = 0;
  int16_t beta_i = 0;
  int8_t beta_j = 0;
  int8_t llr_ratio_i = 0;
  int8_t llr_prob_i = 0;
  
  // buffer
  accum iorecv_pbit[(r_win<<1)]; //store parity bit, for Le (2*windows)
  accum local_deltabuff[(r_win<<1)][16]; //local delta buffer for 2*windows
  accum local_alphabuff[r_win][8];
  accum local_betabuff[r_win][8];
  accum temp_sysbit;
  accum temp_ln_ap1;
  accum temp_ln_ap0;
  accum noise_var;
  accum_int_t temp_accumint; //temp variable for manipulating bits between accum and int type
  accum llr_buff[64];
  accum temp_conv;
  
  // flag
  uint8_t calcLLR;
  //uint8_t iorecv_hold = GEN_FLAG_DOWN; 
  uint8_t deltasend0_hold = GEN_FLAG_DOWN;
  uint8_t deltasend1_hold = GEN_FLAG_DOWN; 
  uint8_t alpharecv_done = GEN_FLAG_DOWN;
  uint8_t betarecv_done = GEN_FLAG_DOWN;
  uint8_t llrsend_io_hold = GEN_FLAG_DOWN;
  uint8_t llrsend_nc0_hold = GEN_FLAG_DOWN;
  uint8_t llrsend_nc1_hold = GEN_FLAG_DOWN;
  uint8_t llrproc_hold = GEN_FLAG_DOWN;
  uint32_t flag_pkt_type_io = EMPTY_HDR;
  uint32_t flag_pkt_type_nc0 = EMPTY_HDR;
  uint32_t flag_pkt_type_nc1 = EMPTY_HDR;
  
  // ## Init phase ##  
  set_trellis();
  pinMode_output(ledpin);
  
  // ## Loop phase ##
  while(1){
    
    // ## DTRMN_PKT_TYPE
    // Examine packet header - IO core
    if( (noc_NC_rxbuff0_av==1) && (flag_pkt_type_io==EMPTY_HDR) )
      flag_pkt_type_io = NC_NOC_RX_BUFF0;

    // Examine packet header - NC0 core
    if( (noc_NC_rxbuff1_av==1) && (flag_pkt_type_nc0==EMPTY_HDR) )
      flag_pkt_type_nc0 = NC_NOC_RX_BUFF1;

    // Examine packet header - NC1 core
    if( (noc_NC_rxbuff2_av==1) && (flag_pkt_type_nc1==EMPTY_HDR) )
      flag_pkt_type_nc1 = NC_NOC_RX_BUFF2;
        
    // ## RECV_CTRL_IO
    //receive calcLLR flag and noise_var from IO core
    if( flag_pkt_type_io==IO_CHNLCTRL_HDR && (noc_NC_rxbuff0_av==1) && (noc_getNC_rxbuff0_count>=2)  ){  
      // recv packet
      calcLLR = NC_NOC_RX_BUFF0;      
      temp_accumint.int_cont = NC_NOC_RX_BUFF0;
      noise_var = temp_accumint.accum_cont;
      
      // send ACK
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF0 = calcLLR;

      // set flag
      flag_pkt_type_io = EMPTY_HDR;
    }
    
    // ## RECV_BITS_IO
    // Receive data info from IO core and calc delta
    if( (flag_pkt_type_io==IO_BITS_HDR) && (noc_NC_rxbuff0_av==1) && (noc_getNC_rxbuff0_count>=4) ){      
      
      // receive sysbit, parbit, apriori from NoC
      temp_accumint.int_cont = NC_NOC_RX_BUFF0; 
      temp_sysbit = temp_accumint.accum_cont;
      
      temp_accumint.int_cont = NC_NOC_RX_BUFF0; 
      iorecv_pbit[iorecv_i & ((r_win<<1)-1)] = temp_accumint.accum_cont;
      
      temp_accumint.int_cont = NC_NOC_RX_BUFF0; 
      temp_ln_ap1 = temp_accumint.accum_cont;
      
      temp_accumint.int_cont = NC_NOC_RX_BUFF0;
      temp_ln_ap0 = temp_accumint.accum_cont;
            
      // calculate, store delta
      r_turbodec_dcalc(temp_sysbit, iorecv_pbit[iorecv_i & ((r_win<<1)-1)], local_deltabuff[iorecv_i & ((r_win<<1)-1)],
                       temp_ln_ap1, temp_ln_ap0, noise_var);
      
      iorecv_i++;
      
      // send back ACK
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF0 = DSP_BITS_ACK_HDR; // Header - bits ack
      
      // set flag
      flag_pkt_type_io = EMPTY_HDR; //clear packet type's flag
      
      // blink LED
      //if( (iorecv_i & (r_win-1))==0 )
        //GPIO_BTGL = 0x1 << ledpin;
      
      //stop receiving if it received two windows and none decoded completely
      //if( iorecv_i==256 || ((iorecv_i&(r_win-1))== 0 && ((iorecv_i>>r_win_mul)-(retllr_i>>r_win_mul))>=2) )
        //iorecv_hold = 1;
    }
    
    // ## RECV_LLR_R_ACK_IO
    if( flag_pkt_type_io==IO_LLR_R_ACK_HDR ){
      // set flag
      llrsend_io_hold = GEN_FLAG_DOWN;
      flag_pkt_type_io = EMPTY_HDR;
    }

    // ## RECV_LLR_P_ACK_IO
    if( flag_pkt_type_io==IO_LLR_P_ACK_HDR ){
      // set flag
      llrproc_hold = GEN_FLAG_DOWN;
      flag_pkt_type_io = EMPTY_HDR;
    } 

    // ## RECV_DELTA_ACK_NC0
    if( flag_pkt_type_nc0==NC_DELTA_ACK_HDR ){
      // set flag
      if(deltasend0_hold!=GEN_FLAG_HOLD)
        deltasend0_hold = GEN_FLAG_DOWN;
      flag_pkt_type_nc0 = EMPTY_HDR;
    }

    // ## RECV_DELTA_ACK_NC1
    if( flag_pkt_type_nc1==NC_DELTA_ACK_HDR ){
      // set flag
      if(deltasend1_hold!=GEN_FLAG_HOLD)
        deltasend1_hold = GEN_FLAG_DOWN;
      flag_pkt_type_nc1 = EMPTY_HDR;
    }

    // ## RECV_ALPHA_NC0
    if( (flag_pkt_type_nc0==NC_AB_HDR) && (noc_NC_rxbuff1_av==1) && (noc_getNC_rxbuff1_count>=4) ){
      // recv packet
      for(int8_t i=0; i<4; i++){
        while(noc_NC_rxbuff1_av!=1)__NOP();
        temp_accumint.int_cont = NC_NOC_RX_BUFF1;
        local_alphabuff[alpha_i][alpha_j] = temp_accumint.accum_cont;
        alpha_j++;
        alpha_j &= 7;
        if(alpha_j==0){
          alpha_i++;
          alpha_i &= 31;
          if(alpha_i==0){
            alpharecv_done = GEN_FLAG_UP;
            //digitalWrite_high(ledpin);
          }
        }
      }

      // send ack
      while(noc_NC_txbuff_isfull==1)__NOP();
      NC_NOC_TX_BUFF1 = DSP_AB_ACK_HDR;

      // set flag
      flag_pkt_type_nc0 = EMPTY_HDR;
    }  

    // ## RECV_BETA_NC1
    if( (flag_pkt_type_nc1==NC_AB_HDR) && (noc_NC_rxbuff2_av==1) && (noc_getNC_rxbuff2_count>=4) ){
      // recv packet
      for(int8_t i=0; i<4; i++){
        while(noc_NC_rxbuff2_av!=1)__NOP();
        temp_accumint.int_cont = NC_NOC_RX_BUFF2;
        local_betabuff[beta_i][beta_j] = temp_accumint.accum_cont;
        beta_j++;
        beta_j &= 7;
        if(beta_j==0){
          beta_i++;
          beta_i &= 31;
          if(beta_i==0){
            betarecv_done = GEN_FLAG_UP;
            //digitalWrite_high(ledpin);
          }
        }
      }

      // send ack
      while(noc_NC_txbuff_isfull==1)__NOP();
      NC_NOC_TX_BUFF2 = DSP_AB_ACK_HDR;

      // set flag
      flag_pkt_type_nc1 = EMPTY_HDR;
    }

    // ## RECV_LLR_R_ACK_NC0
    if( flag_pkt_type_nc0==NC_LLR_R_ACK_HDR ){
      // set flag
      llrsend_nc0_hold = GEN_FLAG_DOWN;
      flag_pkt_type_nc0 = EMPTY_HDR;
    }

    // ## RECV_LLR_R_ACK_NC1
    if( flag_pkt_type_nc1==NC_LLR_R_ACK_HDR ){
      // set flag
      llrsend_nc1_hold = GEN_FLAG_DOWN;
      flag_pkt_type_nc1 = EMPTY_HDR;
    } 
    
    // ## SEND_DELTA_NC
    if( (iorecv_i>delta_i) && (deltasend0_hold==GEN_FLAG_DOWN) && (deltasend1_hold==GEN_FLAG_DOWN) ){
      // send packet
      // - header
      while(noc_NC_txbuff_isfull==1)__NOP();
      NC_NOC_TX_BUFF1 = NC_NOC_TX_BUFF2 = DSP_DELTA_HDR;
      // - payload
      for(int8_t i=0; i<4; i++){
        temp_accumint.accum_cont = local_deltabuff[delta_i & ((r_win<<1)-1)][delta_j]; 
        while(noc_NC_txbuff_isfull==1)__NOP();
        NC_NOC_TX_BUFF1 = NC_NOC_TX_BUFF2 = temp_accumint.int_cont;
        delta_j++;
        delta_j &= 15;
        if( delta_j==0 ){
          delta_i++;
          if( (delta_i & (r_win-1))==0 )
            deltasend0_hold = deltasend1_hold = GEN_FLAG_HOLD;
        }
      }

      // set flag
      if( (deltasend0_hold!=GEN_FLAG_HOLD) && (deltasend1_hold!=GEN_FLAG_HOLD) ) 
        deltasend0_hold = deltasend1_hold = GEN_FLAG_UP;
    }
    //if( (alpharecv_done!=GEN_FLAG_DOWN) && (betarecv_done!=GEN_FLAG_DOWN) )
      //digitalWrite_high(ledpin);
    // ## CALC_SEND_LLR_RATIO
    // calculate LLR in ratio format, conversion to probability workload is shared among cores 
    if( (alpharecv_done!=GEN_FLAG_DOWN) && (betarecv_done!=GEN_FLAG_DOWN) &&
        (llrsend_io_hold ==GEN_FLAG_DOWN) && (llrsend_nc0_hold ==GEN_FLAG_DOWN) && (llrsend_nc1_hold ==GEN_FLAG_DOWN) ){

      // calculate LLR_ratio and send
      // - IO
      temp_accumint.accum_cont = r_turbodec_llrcalc(local_deltabuff[retllr_i & ((r_win<<1)-1)], local_alphabuff[retllr_i & (r_win-1)],
                                   local_betabuff[retllr_i & (r_win-1)], iorecv_pbit[retllr_i & ((r_win<<1)-1)], calcLLR, noise_var);

      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF0 = DSP_LLR_R_HDR;
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF0 = calcLLR;
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF0 = temp_accumint.int_cont;
      llrsend_io_hold = GEN_FLAG_UP;
      retllr_i++;
      
      // - NC0
      temp_accumint.accum_cont = r_turbodec_llrcalc(local_deltabuff[retllr_i & ((r_win<<1)-1)], local_alphabuff[retllr_i & (r_win-1)],
                                   local_betabuff[retllr_i & (r_win-1)], iorecv_pbit[retllr_i & ((r_win<<1)-1)], calcLLR, noise_var);

      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF1 = DSP_LLR_R_HDR;
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF1 = calcLLR;
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF1 = temp_accumint.int_cont;
      llrsend_nc0_hold = GEN_FLAG_UP;
      retllr_i++;
      
      // - NC1
      temp_accumint.accum_cont = r_turbodec_llrcalc(local_deltabuff[retllr_i & ((r_win<<1)-1)], local_alphabuff[retllr_i & (r_win-1)],
                                   local_betabuff[retllr_i & (r_win-1)], iorecv_pbit[retllr_i & ((r_win<<1)-1)], calcLLR, noise_var);

      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF2 = DSP_LLR_R_HDR;
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF2 = calcLLR;
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF2 = temp_accumint.int_cont;
      llrsend_nc1_hold = GEN_FLAG_UP;
      retllr_i++;
      
      // - DSP
      temp_accumint.accum_cont = r_turbodec_llrcalc(local_deltabuff[retllr_i & ((r_win<<1)-1)], local_alphabuff[retllr_i & (r_win-1)],
                                   local_betabuff[retllr_i & (r_win-1)], iorecv_pbit[retllr_i & ((r_win<<1)-1)], calcLLR, noise_var);

      while(noc_NC_txbuff_isfull==1)__NOP(); llr_buff[llr_ratio_i++] = temp_accumint.accum_cont;
      retllr_i++;
      
      //blink LED
      if((retllr_i & (r_win-1))==0)
        GPIO_BTGL = 0x1 << ledpin;
      //if(retllr_i > 32)  
        //GPIO_BTGL = 0x1 << ledpin;
      
      // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //allow sending next window's delta
      //if( ((delta_i & (r_win-1))==0) && (((delta_i>>r_win_mul)-(retllr_i>>r_win_mul))<2))
        //deltasend_hold = 0;
      
      //after it decoded a full window, clear flags and reset counters
      if((retllr_i & (r_win-1))==0){
        alpharecv_done = betarecv_done = GEN_FLAG_DOWN;
        deltasend0_hold = deltasend1_hold = GEN_FLAG_DOWN;
      }
      
      //if( (iorecv_i<256) && ((retllr_i&(r_win-1))== 0) && (((iorecv_i>>r_win_mul)-(retllr_i>>r_win_mul))<2) )
        //iorecv_hold = 0;
      
      if(retllr_i==256)
        iorecv_i = delta_i = retllr_i = 0; //reset counters
    
    } 

    // ## CONVERT_LLR_FORMAT
    if( (llr_ratio_i > llr_prob_i) && (llrproc_hold==GEN_FLAG_DOWN) ){

      temp_conv = llr_buff[llr_prob_i];
      
      //conversion is needed for Le calculation
      if(!calcLLR){
        temp_conv = exp_approx(temp_conv);
        temp_conv /= (1+temp_conv);
        if(temp_conv > 0.99990)
          temp_conv = 0.99990;
        else if(temp_conv < 0.0001)
          temp_conv = 0.0001;
      }
     
      // send converted/non-converted LLR/Le to IO core
      while(noc_NC_txbuff_isfull==1)__NOP();
      NC_NOC_TX_BUFF0 = DSP_LLR_P_HDR;
      while(noc_NC_txbuff_isfull==1)__NOP();
      temp_accumint.accum_cont = temp_conv;
      NC_NOC_TX_BUFF0 = temp_accumint.int_cont;
      
      llr_prob_i++;
      
      llrproc_hold = GEN_FLAG_UP;

      if( llr_prob_i==64 )
        llr_ratio_i = llr_prob_i = 0;
    }
    
  } 

  return 0;

}