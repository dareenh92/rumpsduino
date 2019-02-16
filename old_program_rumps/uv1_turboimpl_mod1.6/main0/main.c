//------------------------------------------------------------------------------

//

// Main Program
// Application    : turboimpl_ioc
// Core           : IO Core
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > recv input seq, govern operations

#include "main.h"
#include "turbo_rumps_c0.h"

#define TURBO_MAX_ITR 1

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
const uint8_t ledpin = 18;

// DRP Interleaver LUT
static const uint8_t drp_lut[256] = {12, 38, 3, 25, 64, 90, 55, 77, 116, 142, 107, 129, 168, 194, 159, 181, 220, 246, 211, 233, 16, 42, 7, 29, 68, 94, 59, 81, 120, 146, 111, 133, 172, 198, 163, 185, 224, 250, 215, 237, 20, 46, 11, 33, 72, 98, 63, 85, 124, 150, 115, 137, 176, 202, 167, 189, 228, 254, 219, 241, 24, 50, 15, 37, 76, 102, 67, 89, 128, 154, 119, 141, 180, 206, 171, 193, 232, 2, 223, 245, 28, 54, 19, 41, 80, 106, 71, 93, 132, 158, 123, 145, 184, 210, 175, 197, 236, 6, 227, 249, 32, 58, 23, 45, 84, 110, 75, 97, 136, 162, 127, 149, 188, 214, 179, 201, 240, 10, 231, 253, 36, 62, 27, 49, 88, 114, 79, 101, 140, 166, 131, 153, 192, 218, 183, 205, 244, 14, 235, 1, 40, 66, 31, 53, 92, 118, 83, 105, 144, 170, 135, 157, 196, 222, 187, 209, 248, 18, 239, 5, 44, 70, 35, 57, 96, 122, 87, 109, 148, 174, 139, 161, 200, 226, 191, 213, 252, 22, 243, 9, 48, 74, 39, 61, 100, 126, 91, 113, 152, 178, 143, 165, 204, 230, 195, 217, 0, 26, 247, 13, 52, 78, 43, 65, 104, 130, 95, 117, 156, 182, 147, 169, 208, 234, 199, 221, 4, 30, 251, 17, 56, 82, 47, 69, 108, 134, 99, 121, 160, 186, 151, 173, 212, 238, 203, 225, 8, 34, 255, 21, 60, 86, 51, 73, 112, 138, 103, 125, 164, 190, 155, 177, 216, 242, 207, 229};

void main_scheduler (void);
//void ioc_recv_uart(); //read bits from UART

//------------------------------------------------------------------------------

int main(void)
{
  // ## Variables ##
  // counter
  int16_t recv_i = 0;
  int8_t temp_recv_i = 0;
  uint8_t llrp_ctr_io = 0;
  uint8_t llrp_ctr_nc0 = 0;
  uint8_t llrp_ctr_nc1 = 0;
  uint8_t llrp_ctr_dsp = 0;
  int16_t dspsent_i = 0;
  int16_t dec_i = 0;
  //int16_t expected_llridx = 0;
  uint8_t halfitr_ctr = 0; //track number of iteration(per-half itr)
  uint8_t I_last_send = intrlv_Im1; //latest permutated index for DRP interleaver
  uint8_t I_last_recv = intrlv_Im1; //latest permutated index for DRP interleaver
  
  // flag
  uint8_t recv_done = GEN_FLAG_DOWN; //flag of a data frame is fully received
  uint8_t calcLLR = 0; 
  uint8_t dspsend_hold = 0; //flag to hold sending data to DSP core
  uint8_t llrp_recv_io = GEN_FLAG_DOWN; 
  uint8_t llrp_recv_nc0 = GEN_FLAG_DOWN;
  uint8_t llrp_recv_nc1 = GEN_FLAG_DOWN;
  uint8_t llrp_recv_dsp = GEN_FLAG_DOWN;
  //uint32_t llridx_mask[8] = {0};
  uint32_t flag_pkt_type = EMPTY_HDR;

  uint8_t dec_stat = 0; //flag if dec1 (0) / dec2 (1) is active now
  
  // buffer
  accum noise_var = 100;
  accum recv_sbit[256];   //received systematic bit
  accum recv_pbit1[256]; //received parity bit dec1
  accum recv_pbit2[256];  //received parity bit dec2
  accum LLR_Le_arr[256]; //stores LLR / Le per half iteration
  accum LLR_p_corebuff[4];
  accum_int_t temp_accumint; //temp variable for manipulating bits between accum and int type
  accum temp_conv;
    
  // ## Init phase ##  
  pinMode_output(ledpin);
  set_trellis();
  init_uart();

  // ## Loop phase ##  
  while(1){
    
    // ## GET_CHNL_DATA
    // Get data from channel - currently from uart (without noise!!)
    while(recv_done==GEN_FLAG_DOWN){
      char temp_uart;
      
      if( _getchar(&temp_uart) ){
        if(temp_recv_i==0){
          recv_sbit[recv_i] = temp_uart; 
          recv_sbit[recv_i] = (recv_sbit[recv_i]<<1) - 1;
          //temp_accumint.accum_cont = recv_sbit[recv_i];
          temp_recv_i++;
        }
        else if(temp_recv_i==1){
          recv_pbit1[recv_i] = temp_uart;
          recv_pbit1[recv_i] = (recv_pbit1[recv_i]<<1) - 1;
          //temp_accumint.accum_cont = recv_pbit1[recv_i];
          temp_recv_i++;
        }
        else if(temp_recv_i==2){
          recv_pbit2[recv_i] = temp_uart;
          recv_pbit2[recv_i] = (recv_pbit2[recv_i]<<1) - 1;
          //temp_accumint.accum_cont = recv_pbit2[recv_i];
          temp_recv_i = 0;
          recv_i++;
        }
      }
      
      //increment received bit count, mark if a complete frame is received
      if(recv_i==256){
        recv_done = GEN_FLAG_UP;
        noise_var = 1/noise_var;
      }
    }

    // ## DTRMN_PKT_TYPE
    // Examine packet header - DSP core
    if( (noc_IC_rxbuff3_av==1) && (flag_pkt_type==EMPTY_HDR) )
      flag_pkt_type = IC_NOC_RX_BUFF3;
    
    // packet from NC cores contains only LLR_p
    if( (noc_IC_rxbuff1_av==1) ){
      // receive data
      temp_accumint.int_cont = IC_NOC_RX_BUFF1;
      LLR_p_corebuff[1] = temp_accumint.accum_cont;
      llrp_ctr_nc0++;

      // send ack
      if( (llrp_ctr_nc0 & 7)==0)
        IC_NOC_TX_BUFF1 = IO_LLR_P_ACK_HDR;

      // set flag
      llrp_recv_nc0 = GEN_FLAG_UP;
    }

    if( (noc_IC_rxbuff2_av==1) ){
      // receive data
      temp_accumint.int_cont = IC_NOC_RX_BUFF2;
      LLR_p_corebuff[2] = temp_accumint.accum_cont;
      llrp_ctr_nc1++;
      
      // send ack
      if( (llrp_ctr_nc1 & 7)==0)
        IC_NOC_TX_BUFF2 = IO_LLR_P_ACK_HDR;
    
      // set flag
      llrp_recv_nc1 = GEN_FLAG_UP;
    }

    // ## RECV_BITS_ACK_DSP
    if( (flag_pkt_type==DSP_BITS_ACK_HDR) ){
      // set flag
      flag_pkt_type = EMPTY_HDR;
      dspsend_hold = GEN_FLAG_DOWN;
      
      //dspsend_hold criteria #2- dspsent_i is a multiple of r_win
      //hold sending if it sent out two windows, and none of them returned completely
      //or if it has sent all bits of a frame
      if( dspsent_i==256 || ((dspsent_i&(r_win-1))== 0 && ((dspsent_i>>r_win_mul)-(dec_i>>r_win_mul))>=2) )
        dspsend_hold = GEN_FLAG_HOLD;
    }

    // ## RECV_LLR_P_DSP
    if( (flag_pkt_type==DSP_LLR_P_HDR) && (noc_IC_rxbuff3_av==1) ){
      // receive data
      temp_accumint.int_cont = IC_NOC_RX_BUFF3;
      LLR_p_corebuff[3] = temp_accumint.accum_cont;
      llrp_ctr_dsp++;
      
      // send ack
      IC_NOC_TX_BUFF3 = IO_LLR_P_ACK_HDR;

      // set flag
      flag_pkt_type = EMPTY_HDR;
      llrp_recv_dsp = GEN_FLAG_UP;
    }

    // ## RECV_LLR_R_DSP
    if( flag_pkt_type==DSP_LLR_R_HDR && (noc_IC_rxbuff3_av==1) && (noc_getIC_rxbuff3_count>=2)){
      // retrieve calcLLR and LLR_ratio
      uint8_t temp_calcLLR = IC_NOC_RX_BUFF3;
      temp_accumint.int_cont = IC_NOC_RX_BUFF3;

      // send back ACK to DSP core
      while(noc_IC_txbuff_isfull==1)__NOP(); IC_NOC_TX_BUFF3 = IO_LLR_R_ACK_HDR;

      // convert LLR from ratio to prob format
      temp_conv = temp_accumint.accum_cont;
      if(!temp_calcLLR){
        temp_conv = exp_approx(temp_conv);
        temp_conv /= (1+temp_conv);
        if(temp_conv > 0.99990)
          temp_conv = 0.99990;
        else if(temp_conv < 0.0001)
          temp_conv = 0.0001;
      }

      // store data - in same format as other cores' LLR_p
      LLR_p_corebuff[0] = temp_conv;
      llrp_ctr_io++;

      // set flag
      flag_pkt_type = EMPTY_HDR;
      llrp_recv_io = GEN_FLAG_UP;
    }

    // ## ARR_LLR_P
    // Arrange received LLR_p from cores into common buffer
    if( (llrp_recv_io==GEN_FLAG_UP) || (llrp_recv_nc0==GEN_FLAG_UP) ||
        (llrp_recv_nc1==GEN_FLAG_UP) || (llrp_recv_dsp==GEN_FLAG_UP) ){ 
      
      //llridx = I_last_recv = drp_idxcalc(r_K, r_M, I_last_recv, (unsigned char*)intrlv_P, llridx);
      //llridx_mask[(temp_idx >> 5)] |= (1 << (temp_idx & 31)); // set mask - keep track of processed index
      
      // arrange for each core's LLR_p
      uint8_t llridx;

      if(llrp_recv_io==GEN_FLAG_UP){ // from IO core
        llridx = llrp_ctr_io - 1; // index starts at 0
        llridx = (llridx << 2);
        /*
        if(dec_stat==1){ // interleave for decoder 2
          if(llridx == expected_llridx){
            llridx = I_last_recv = drp_idxcalc(r_K, r_M, I_last_recv, (unsigned char*)intrlv_P, llridx);
            expected_llridx++;
            
          }
          else if(llridx > expected_llridx){
            uint8_t temp_ilast = I_last_recv;
            for(int i=expected_llridx; i<=llridx; i++){
              temp_ilast =  drp_idxcalc(r_K, r_M, temp_ilast, (unsigned char*)intrlv_P, i);
            }
          }
        }
        */
        if(dec_stat==1)
          llridx = drp_lut[llridx];
        
        // store LLR / Le
        LLR_Le_arr[llridx] = LLR_p_corebuff[0];
        dec_i++;
        llrp_ctr_io &= 63;

        // set flag
        llrp_recv_io = GEN_FLAG_DOWN;
      }
      
      if(llrp_recv_nc0==GEN_FLAG_UP){ // from NC0 core
        llridx = llrp_ctr_nc0 - 1; // index starts at 0
        llridx = (llridx << 2) + 1;

        if(dec_stat==1)
          llridx = drp_lut[llridx];
        
        // store LLR / Le
        LLR_Le_arr[llridx] = LLR_p_corebuff[1];
        dec_i++;
        llrp_ctr_nc0 &= 63;
        
        // set flag
        llrp_recv_nc0 = GEN_FLAG_DOWN;
      }

      if(llrp_recv_nc1==GEN_FLAG_UP){ // from NC1 core
        llridx = llrp_ctr_nc1 - 1; // index starts at 0
        llridx = (llridx << 2) + 2;

        if(dec_stat==1)
          llridx = drp_lut[llridx];
        
        // store LLR / Le
        LLR_Le_arr[llridx] = LLR_p_corebuff[2];
        dec_i++;
        llrp_ctr_nc1 &= 63;

        // set flag
        llrp_recv_nc1 = GEN_FLAG_DOWN;
      }

      if(llrp_recv_dsp==GEN_FLAG_UP){ // from DSP core
        llridx = llrp_ctr_dsp - 1; // index starts at 0
        llridx = (llridx << 2) + 3;

        if(dec_stat==1)
          llridx = drp_lut[llridx];
        
        // store LLR / Le
        LLR_Le_arr[llridx] = LLR_p_corebuff[3];
        dec_i++;
        llrp_ctr_dsp &= 63;

        // set flag
        llrp_recv_dsp = GEN_FLAG_DOWN;
      }
      
      //blink led
      if( (dec_i & (r_win-1))==0 )
        GPIO_BTGL = 0x1 << ledpin;
      //if( (llrp_ctr_nc0>8) )
        //digitalWrite_high(ledpin);
      
      //when on hold, 1 window returned allows 1 more window to be sent
      if( dspsent_i<256 && dspsend_hold==GEN_FLAG_HOLD && (dec_i&(r_win-1))==0 && ((dspsent_i>>r_win_mul)-(dec_i>>r_win_mul))<2 )
        dspsend_hold = GEN_FLAG_DOWN;
        
      // Switch between decoders per half iteration
      if(dec_i==256){
        dec_i = dspsent_i = 0; //reset counter
        dspsend_hold = GEN_FLAG_DOWN; //allow sending to DSP
        
        dec_stat = 1 - dec_stat; //toggle active decoder
        
        if(dec_stat==0) //reset interleaver recursive count
          I_last_send = I_last_recv = intrlv_Im1;
        
        if(++halfitr_ctr == ((TURBO_MAX_ITR<<1)-1)){ //signal DSP core to calc LLR
          calcLLR = 1; 
          /*
          char uart_buff;
          //output decision based on LLR
          for(short i=0; i<256; i++){
            if(LLR_Le_arr[i]>0.5)
              uart_buff = 1;  
            else
              uart_buff = 0;
          
            _putchar(uart_buff); //send through UART
            while( !_getchar(&uart_buff) )__NOP(); //wait for ACK
          }
          */
        }
        //to do on the final iteration - move on to new frame
        if(halfitr_ctr == (TURBO_MAX_ITR<<1)){
          recv_i = halfitr_ctr = 0; // reset counters
          recv_done = GEN_FLAG_DOWN; // allow receiving channel data
          calcLLR = 0; // signal DSP core to calc Le
          
          char uart_buff;
          //output decision based on LLR
          for(short i=0; i<256; i++){
            if(LLR_Le_arr[i]>0)
              uart_buff = 1;  
            else
              uart_buff = 0;
          
            _putchar(uart_buff); //send through UART
            while( !_getchar(&uart_buff) )__NOP(); //wait for ACK
          }
          
        }
      }
      
    }

    // ## SEND_BITS_DSP
    // Send channel control and bits to DSP core
    if( (recv_i>dspsent_i) && (dspsend_hold==GEN_FLAG_DOWN) ){
      // ** Send control bits, if this is first bit of an iteration
      if(dspsent_i==0){
        //send calcLLR and noise_var to DSP core
        while(noc_IC_txbuff_isfull==1)__NOP(); IC_NOC_TX_BUFF3 = IO_CHNLCTRL_HDR; // Header - chnl_control
        while(noc_IC_txbuff_isfull==1)__NOP(); IC_NOC_TX_BUFF3 = calcLLR;
        while(noc_IC_txbuff_isfull==1)__NOP(); temp_accumint.accum_cont = noise_var; IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
        
        //wait for ACK -  since it is blocking, no need to differentiate this one
        while(noc_IC_rxbuff3_av!=1)__NOP();
        int temp_ack = IC_NOC_RX_BUFF3;
      }
      
      // ** Send data bits
      while(noc_IC_txbuff_isfull==1)__NOP(); IC_NOC_TX_BUFF3 = IO_BITS_HDR; // Header - bits
            
      //if dec1 is active
      if(dec_stat==0){
        while(noc_IC_txbuff_isfull==1)__NOP(); temp_accumint.accum_cont = recv_sbit[dspsent_i]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //systematic bit
        while(noc_IC_txbuff_isfull==1)__NOP(); temp_accumint.accum_cont = recv_pbit1[dspsent_i]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //parity bit
        
        //apriori prob - ln(apriori) and ln(1-apriori)
        if(halfitr_ctr==0){ 
          while(noc_IC_txbuff_isfull==1)__NOP();
          temp_accumint.accum_cont = -0.6931;
          IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
          while(noc_IC_txbuff_isfull==1)__NOP();
          IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
        }
        else{
          while(noc_IC_txbuff_isfull==1)__NOP();
          temp_accumint.accum_cont = ln_approx(LLR_Le_arr[dspsent_i]);
          IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
          while(noc_IC_txbuff_isfull==1)__NOP();
          temp_accumint.accum_cont = ln_approx(1-LLR_Le_arr[dspsent_i]);
          IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
        }
      }
      
      //if dec2 is active
      else{ 
        //I_last_send = drp_idxcalc(r_K, r_M, I_last_send, (unsigned char*)intrlv_P, dspsent_i);
        I_last_send = drp_lut[dspsent_i];
        while(noc_IC_txbuff_isfull==1)__NOP(); temp_accumint.accum_cont = recv_sbit[I_last_send]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //systematic bit
        while(noc_IC_txbuff_isfull==1)__NOP(); temp_accumint.accum_cont = recv_pbit2[dspsent_i]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //parity bit
        
        //apriori prob - ln(apriori) and ln(1-apriori)
        if(halfitr_ctr==0){
          while(noc_IC_txbuff_isfull==1)__NOP();
          temp_accumint.accum_cont = -0.6931;
          IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
          while(noc_IC_txbuff_isfull==1)__NOP();
          IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
        }
        else{
          while(noc_IC_txbuff_isfull==1)__NOP();
          temp_accumint.accum_cont = ln_approx(LLR_Le_arr[I_last_send]); 
          IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
          while(noc_IC_txbuff_isfull==1)__NOP();
          temp_accumint.accum_cont = ln_approx(1-LLR_Le_arr[I_last_send]); 
          IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
        }
      }
                  
      //increment sent bit counter
      dspsent_i++;
      
      //hold sending to dsp until ACK received for each packet
      dspsend_hold = GEN_FLAG_UP;
      
      //blink led
      //if((dspsent_i & (r_win-1))==0)
        //digitalWrite_high(ledpin);
        //GPIO_BTGL = 0x1 << ledpin;
      
    } 
    
  }

  return 0;

}

