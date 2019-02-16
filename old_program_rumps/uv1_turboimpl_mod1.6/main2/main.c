//------------------------------------------------------------------------------

//

// Main Program
// Application    : turboimpl_nc1
// Core           : Normal Core 1
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > calc b, convert LLR ratio->prob

#include "main.h"
#include "turbo_rumps_c2.h"

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

// IO pins
const uint8_t ledpin = 0;


void main_scheduler (void);


//------------------------------------------------------------------------------

int main(void)

{
  // ## Variables ##
  // counter
  int16_t drecv_i = 0;
  int8_t drecv_j = 0;
  int8_t betacalc_i = 31; //backward recursive
  int16_t bsent_i = 0;
  int8_t bsent_j = 0;
  int8_t llr_ratio_i = 0;
  int8_t llr_prob_i = 0;
  
  // flag
  uint8_t ready_betacalc = GEN_FLAG_DOWN;
  uint8_t ready_betasend = GEN_FLAG_DOWN;
  uint8_t beta_iswinend = GEN_FLAG_DOWN;
  uint8_t calcLLR = 0;
  uint8_t llrproc_hold = GEN_FLAG_DOWN;
  uint32_t flag_pkt_type = EMPTY_HDR;
  
  // buffer
  accum local_deltabuff[r_win][16];
  accum local_betabuff[r_win][8];
  accum_int_t temp_accumint; //temp variable for manipulating bits between accum and int type
  accum llr_buff[64];
  accum temp_conv;

  // ## Init phase ## 
  pinMode_output(ledpin); 
  set_trellis();
  for(int8_t i=0; i<8; i++) // Init beta
    local_betabuff[r_win-1][i] = -2.0794;

  // ## Loop phase ##  
  while(1){    

    // ## DTRMN_PKT_TYPE
    // Examine packet header - DSP core
    if( (noc_NC_rxbuff3_av==1) && (flag_pkt_type==EMPTY_HDR) )
      flag_pkt_type = NC_NOC_RX_BUFF3;
    
    // packet from IO core contains only ack for converted LLR
    if( noc_NC_rxbuff0_av==1 ){
      int temp_ack = NC_NOC_RX_BUFF0;
      llrproc_hold = GEN_FLAG_DOWN;
    }
    
    // ## RECV_DELTA_DSP - per 4 flits
    if( (flag_pkt_type==DSP_DELTA_HDR) && (noc_getNC_rxbuff3_count>=4) ){
      // recv packet
      for(int8_t i=0; i<4; i++){
        while(noc_NC_rxbuff3_av!=1)__NOP();
        temp_accumint.int_cont = NC_NOC_RX_BUFF3;
        local_deltabuff[drecv_i & (r_win-1)][drecv_j] = temp_accumint.accum_cont;
        drecv_j++;
        drecv_j &= 15;
        if(drecv_j==0){
          drecv_i++;
          drecv_i &= 255;
          if( (drecv_i & (r_win-1))==0 )
            ready_betacalc = GEN_FLAG_UP;
        }
      }

      // send ACK
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF3 = NC_DELTA_ACK_HDR;    

      // set flag
      flag_pkt_type = EMPTY_HDR;
    }
    
    // ## RECV_BETA_ACK
    if( (flag_pkt_type==DSP_AB_ACK_HDR) ){
      // set flag
      flag_pkt_type = EMPTY_HDR;
      // only on a same window
      if( beta_iswinend==GEN_FLAG_DOWN )
        ready_betasend = GEN_FLAG_UP;
    } 

    // ## RECV_LLR_R_DSP
    if( (flag_pkt_type==DSP_LLR_R_HDR) && (noc_NC_rxbuff3_av==1) && (noc_getNC_rxbuff3_count>=2) ){
      // retrieve calcLLR and LLR_ratio
      calcLLR = NC_NOC_RX_BUFF3;
      temp_accumint.int_cont = NC_NOC_RX_BUFF3;
      llr_buff[llr_ratio_i++] = temp_accumint.accum_cont;
      
      // send back ACK to DSP core
      while(noc_NC_txbuff_isfull==1)__NOP(); NC_NOC_TX_BUFF3 = NC_LLR_R_ACK_HDR;

      // set flag
      flag_pkt_type = EMPTY_HDR;
    }

    // ## CALC_BETA
    if( ready_betacalc==GEN_FLAG_UP ){
      // calculate beta - per xk
      for(int8_t i=0; i<31; i++){
        //if(betacalc_i-- == 32) continue; // skip on first iteration
        r_turbodec_bcalc(local_deltabuff[betacalc_i], local_betabuff[betacalc_i], local_betabuff[betacalc_i-1]);
        betacalc_i--;
      }

      // set flag - done calculating
      if( betacalc_i==0 ){
        betacalc_i = 31;
        ready_betacalc = GEN_FLAG_DOWN;
        ready_betasend = GEN_FLAG_UP;
      }
    }
    
    // ## SEND_BETA_DSP - per 4 flits
    if( ready_betasend==GEN_FLAG_UP ){
      // set flag - clear last window marking
      beta_iswinend = GEN_FLAG_DOWN;

      // send packet
      // - header
      while(noc_NC_txbuff_isfull==1)__NOP();
      NC_NOC_TX_BUFF3 = NC_AB_HDR;
      // - payload
      for(int8_t i=0; i<4; i++){
        while(noc_NC_txbuff_isfull==1)__NOP();
        temp_accumint.accum_cont = local_betabuff[bsent_i & (r_win-1)][bsent_j];
        NC_NOC_TX_BUFF3 = temp_accumint.int_cont;
        bsent_j++;
        bsent_j &= 7;
        if( bsent_j==0 ){
          bsent_i++;
          bsent_i &= 255;
          if( (bsent_i & (r_win-1))==0 ){
            beta_iswinend = GEN_FLAG_UP;
            GPIO_BTGL = 0x1 << ledpin;  
          }
        }
      }

      // set flag - wait for ack
      ready_betasend = GEN_FLAG_DOWN;
    }
    
    // ## CONVERT_LLR_FORMAT
    // convert LLR from ratio to prob format
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
      temp_accumint.accum_cont = temp_conv;
      NC_NOC_TX_BUFF0 = temp_accumint.int_cont;
      
      llr_prob_i++;
      
      if( (llr_prob_i & 7)==0 ){
        llrproc_hold = GEN_FLAG_UP;
        if( llr_prob_i==64 )
          llr_ratio_i = llr_prob_i = 0;
      }
    }

  }

   

  return 0;

}