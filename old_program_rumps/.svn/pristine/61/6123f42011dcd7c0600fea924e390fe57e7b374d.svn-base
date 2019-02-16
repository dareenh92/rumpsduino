//------------------------------------------------------------------------------
//
// Main Program
// Application		: turboimpl_ioc_DEMO
// Core						: IO Core
// Purpose
//	- Turbo code implementation on RUMPS401
//  - > recv input seq, govern operations
//  - > This serves as a demo program, thus received data are hardcoded
//      and there will be message displayed on LCD upon complete decoding

#include "main.h"
#include "turbo_rumps_c0.h"
#define TURBO_MAX_ITR 1

#define IO_CHNLCTRL_HDR 1
#define IO_BITS_HDR 2
#define IO_LLRACK_HDR 3

#define DSP_LLR_HDR 1
#define DSP_BITSACK_HDR 2

const unsigned char ledpin = 18;
const unsigned char btnpin = 19;

// hardcoded codeword - demo purpose
// total of 768 bits
// we store 63 bitsin first 12 array elements
int64_t hard_codeword_arr[13];

void main_scheduler (void);
//void ioc_recv_uart(); //read bits from UART

//------------------------------------------------------------------------------
int main(void)
{
  set_trellis();
  
  //init_uart();
  pinMode_output(ledpin);
  pinMode_input(btnpin);
  
  // Init LCD display
  Lcd_Init();
  LCD_Clear(BLACK);
  
  //
  hard_codeword_arr[0] = 0x7570D748E5172106;
  hard_codeword_arr[1] = 0x191D7296EBB05CCA;
  hard_codeword_arr[2] = 0x260B09C8538AC175;
  hard_codeword_arr[3] = 0x58211652DBCF1E94;
  hard_codeword_arr[4] = 0x36031460851F08F4;
  hard_codeword_arr[5] = 0x218313AE0F57F9E3;
  hard_codeword_arr[6] = 0xC6B0256C8AB2D9D;
  hard_codeword_arr[7] = 0x7E92E8C2623B7E22;
  hard_codeword_arr[8] = 0x641CDD38DF5F26FD;
  hard_codeword_arr[9] = 0x5BBA821FC2135476;
  hard_codeword_arr[10] = 0xAB3093ECC3B08BD;
  hard_codeword_arr[11] = 0x476899E240CAE195;
  hard_codeword_arr[12] = 0xC07;
  //
  
	//Enable processor's interrupt
	//* Ext INT 8 - GPIO0
	/*NVIC_SetPriority(8, 0);	
	NVIC_EnableIRQ(8);
  
  //set GPIO interrupt registers
  GPIO_IT_CLR = 0x1; //level triggered
  GPIO_IP_CLR = 0x1; //low-level
  GPIO_IE_SET = 0x1; //GPIO_0*/
  
  // Define variables
	short dspsent_i = 0;	//keeps track of bits sent to DSP core
	short dec_i = 0; 		//keeps track of decoded bits
  short recv_i = 0;		//keeps track of received channel bits 
  short temp_recv_i = 0;
	
	accum LLR_Le_arr[256]; //stores LLR / Le per half iteration
	accum noise_var = 1;
	
  // Channel data's receive buffer
  accum recv_sbit[256]; 	//received systematic bit
  accum recv_pbit1[256]; //received parity bit dec1
  accum recv_pbit2[256];	//received parity bit dec2
		
	unsigned char calcLLR = 0;	
	
	unsigned char dec_stat = 0; //flag if dec1 (0) / dec2 (1) is active now
	unsigned char recv_done = 0; //flag of a data frame is fully received
	unsigned char halfitr_ctr = 0; //track number of iteration(per-half itr)
	unsigned char dspsend_hold = 0; //flag to hold sending data to DSP core
	
	unsigned char I_last_send = intrlv_Im1; //latest permutated index for DRP interleaver
	unsigned char I_last_recv = intrlv_Im1; //latest permutated index for DRP interleaver
  
  unsigned char flag_pkt_type = 0;
  
  accum_int_t temp_accumint; //temp variable for manipulating bits between accum and int type
  
  //Display starting message on LCD
  //char lcd_msg[10] = "Starting";
  //LCD_ShowChar(0x6, 0xf, 'A', 0x1, WHITE, BLACK);
  
  while(1){
    
    // ## GET_CHNL_DATA
    // Get data from channel (Demo purpose - hardcoded)
		if( (recv_done==0) && (digitalRead(btnpin)==0) ){
		
		  //clean LCD on new frame
      LCD_Clear(BLACK);
      char msg[40] = "Start decoding\0";
      LCD_ShowString(0x6, 0x6, msg, WHITE, BLACK);
      
		  //read in harcoded codeword, and convert to bipolar
  	  for(int i=0; i<12; i++){
  		  for(int j=21; j>0; j--){
    		  recv_sbit[recv_i] = (hard_codeword_arr[i] >> (j+(j<<1)-1)) & 0x1; 
  			  recv_pbit1[recv_i] = (hard_codeword_arr[i] >> (j+(j<<1)-2)) & 0x1;
  			  recv_pbit2[recv_i] = (hard_codeword_arr[i] >> (j+(j<<1)-3)) & 0x1;
  			  
			    recv_sbit[recv_i] = (recv_sbit[recv_i]<<1) - 1;
			    recv_pbit1[recv_i] = (recv_pbit1[recv_i]<<1) - 1;
			    recv_pbit2[recv_i] = (recv_pbit2[recv_i]<<1) - 1;
			    
  			  recv_i++;
  		  }
  		}
  		for(int j=4; j>0; j--){
  		  recv_sbit[recv_i] = (hard_codeword_arr[12] >> (j+(j<<1)-1)) & 0x1;
  		  recv_pbit1[recv_i] = (hard_codeword_arr[12] >> (j+(j<<1)-2)) & 0x1;
  		  recv_pbit2[recv_i] = (hard_codeword_arr[12] >> (j+(j<<1)-3)) & 0x1;
  		  
		    recv_sbit[recv_i] = (recv_sbit[recv_i]<<1) - 1;
		    recv_pbit1[recv_i] = (recv_pbit1[recv_i]<<1) - 1;
		    recv_pbit2[recv_i] = (recv_pbit2[recv_i]<<1) - 1;
		    
  		  recv_i++;
  		}
  		
			//increment received bit count, mark if a complete frame is received
			if(recv_i==256){
				recv_done = 1;
			}
		}
    
    // ## DTRMN_PKT_TYPE
    // Examine packet header
    if( ((IC_NOC_CSR0 & 0x40)==0x40) && (flag_pkt_type==0) )
      flag_pkt_type = IC_NOC_RX_BUFF3;
    
    /*// Print on LCD - conf from NC
    if( (IC_NOC_CSR0 & 0x10)==0x10 ){
      int tempbuff = IC_NOC_RX_BUFF2;
      
      char msg[40] = "Normal cores done\0";
      LCD_ShowString(0x6, 0x26, msg, WHITE, BLACK);
    }*/
    
    // ## RECV_LLR_DSP
    // Receive LLR / Le from DSP core accordingly
    // ((IC_NOC_CSR0 & 0x40)==0x40) && (((IC_NOC_CSR2&0xf0000000)>>28)==0x4)
		if( (flag_pkt_type==DSP_LLR_HDR) && ((IC_NOC_CSR0 & 0x40)==0x40) && (((IC_NOC_CSR2&0xf0000000)>>28)>=0x4) ){
			
		  //adjust index accordingly, depends on dec1 / dec2
			for(char i=0; i<4; i++){
  			unsigned char llridx;
  			if(dec_stat==0)
  				llridx = dec_i;
  			else
  				llridx = I_last_recv = drp_idxcalc(r_K, r_M, I_last_recv, (unsigned char*)intrlv_P, dec_i);
  			
  			//store LLR / Le
  			temp_accumint.int_cont = IC_NOC_RX_BUFF3;
  			LLR_Le_arr[llridx] = temp_accumint.accum_cont;
  			dec_i++;
			}
			
		  //send back ACK per x flits
		  while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF3 = IO_LLRACK_HDR; // Header - LLR ack
		  while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF3 = dec_i;
		
		  flag_pkt_type = 0; //clear packet type's flag
			
			//blink led
			//if((dec_i & (r_win-1))==0)
			  //GPIO_BTGL = 0x1 << ledpin;
			
			//when on hold, 1 window returned allows 1 more window to be sent
			if( dspsent_i<256 && dspsend_hold==2 && (dec_i&(r_win-1))== 0 && ((dspsent_i>>r_win_mul)-(dec_i>>r_win_mul))<2 )
				dspsend_hold = 0;
				
  		// Switch between decoders per half iteration
  		if(dec_i==256){
  			dec_i = dspsent_i = 0; //reset counter
  			dspsend_hold = 0; //allow sending to DSP
  			
  			dec_stat = 1 - dec_stat; //toggle active decoder
  			
  			if(dec_stat==0) //reset interleaver recursive count
  				I_last_send = I_last_recv = intrlv_Im1;
  			
  			if(++halfitr_ctr == ((TURBO_MAX_ITR<<1)-1)) //signal DSP core to calc LLR
  				calcLLR = 1; 
      
  			//to do on the final iteration - move on to new frame
  			if(halfitr_ctr == (TURBO_MAX_ITR<<1)){
  				recv_i = recv_done = 0; //allow receiving channel data, reset counter
  				halfitr_ctr = 0; //reset iteration counter
  				calcLLR = 0; //signal DSP core to calc Le
          
          // Print on LCD - conf from NC
          if( (IC_NOC_CSR0 & 0x10)==0x10 ){
            int tempbuff = IC_NOC_RX_BUFF2;
            
            char msg[40] = "Normal cores done\0";
            LCD_ShowString(0x6, 0x26, msg, WHITE, BLACK);
          }
          
          char msg[257] = "DSP core done\0";
          LCD_ShowString(0x6, 0x36, msg, WHITE, BLACK);
          for(int i=0; i<100000; i++)__NOP();
          char msg2[40] = "Finished decoding\0";
          LCD_ShowString(0x6, 0x46, msg2, WHITE, BLACK);
          
  				//output decision based on LLR
  				for(short i=0; i<256; i++){
  					if(LLR_Le_arr[i]>0)
  						msg[i]  = '1';	
  					else
  						msg[i]  = '0';
  				}
          LCD_ShowString(0x6, 0x66, msg, WHITE, BLACK);
  			}
  		}
  		
		}
		
    // ## SEND_BITS_DSP
		// Send bits to DSP core
		
		// check for ACK per 1 bits packet sent
		if( (flag_pkt_type==DSP_BITSACK_HDR) && ((IC_NOC_CSR0 & 0x40)==0x40) ){
		  flag_pkt_type = 0; //clear packet type's flag
		  int temp_ack = IC_NOC_RX_BUFF3;
		  
		  dspsend_hold = 0;
		  
			//dspsend_hold criteria #2- dspsent_i is a multiple of r_win
			//hold sending if it sent out two windows, and none of them returned completely
			//or if it has sent all bits of a frame
		  if( dspsent_i==256 || ((dspsent_i&(r_win-1))== 0 && ((dspsent_i>>r_win_mul)-(dec_i>>r_win_mul))>=2) ){
				dspsend_hold = 2;
  			  
        char msg[40] = "IO Core sent out bits\0";
        LCD_ShowString(0x6, 0x16, msg, WHITE, BLACK);
			}
		}
		
		// Sending part
		if( (recv_i>dspsent_i) && (dspsend_hold==0) ){
			// ** Send control bits, if this is first bit of an iteration
			if(dspsent_i==0){
			  //send calcLLR and noise_var
			  while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF3 = IO_CHNLCTRL_HDR; // Header - chnl_control
			  while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF3 = calcLLR; 
			  
			  //wait for ACK -  since it is blocking, no need to differentiate this one
			  while((NC_NOC_CSR0 & 0x40)!=0x40)__NOP();
			  int temp_ack = IC_NOC_RX_BUFF3;
			  
			}
			
			// ** Send data bits
			while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF3 = IO_BITS_HDR; // Header - bits
						
			//if dec1 is active
			if(dec_stat==0){
				while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = recv_sbit[dspsent_i]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //systematic bit
				while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = recv_pbit1[dspsent_i]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //parity bit
				//apriori prob
				if(halfitr_ctr==0){ 
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = 0.5;
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
				}
				else{
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = LLR_Le_arr[dspsent_i];
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
				}
			}
			
			//if dec2 is active
			else{ 
				I_last_send = drp_idxcalc(r_K, r_M, I_last_send, (unsigned char*)intrlv_P, dspsent_i);
				while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = recv_sbit[I_last_send]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //systematic bit
				while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = recv_pbit2[dspsent_i]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //parity bit
				//apriori prob
				if(halfitr_ctr==0){
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = 0.5;
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
				}
				else{
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = LLR_Le_arr[I_last_send]; 
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
			  }
			}
			
			// send noise_var info - partially as padding
			while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = noise_var; IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
			
			//increment sent bit counter
			dspsent_i++;
			
			//hold sending to dsp until ACK received for each packet
			dspsend_hold = 1;
			
			//blink led
			if((dspsent_i & (r_win-1))==0)
			  GPIO_BTGL = 0x1 << ledpin;
			
		} 
    
  }
  
	return 0;
}
