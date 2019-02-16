//------------------------------------------------------------------------------

//

// Main Program
// Application		: turboimpl_ioc
// Core						: IO Core
// Purpose
//	- Turbo code implementation on RUMPS401
//  - > recv input seq, govern operations

#include "main.h"
#include "turbo_rumps_c0.h"

#define TURBO_MAX_ITR 3

#define IO_CHNLCTRL_HDR 0x1
#define IO_BITS_HDR 0x2
#define IO_LLRACK_HDR 0x3
#define IO_NOVAR_HDR 0x4

#define DSP_LLR_HDR 0x31
#define DSP_BITSACK_HDR 0x32

const unsigned char ledpin = 18;


void main_scheduler (void);
//void ioc_recv_uart(); //read bits from UART


//------------------------------------------------------------------------------

int main(void)

{
  set_trellis();
  //uart_hd_init_uart(0x1);
  init_uart();
  pinMode_output(ledpin);
  
	//Enable processor's interrupt
	//* Ext INT 8 - GPIO0

	/*NVIC_SetPriority(8, 0);	

	NVIC_EnableIRQ(8);
  
  //set GPIO interrupt registers
  GPIO_IT_CLR = 0x1; //level triggered
  GPIO_IP_CLR = 0x1; //low-level
  GPIO_IE_SET = 0x1; //GPIO_0*/

  /*
  //Timer Settings - Lime interfacing
  TM_PR = 0x1; // prescale target, 4MHz 
  TM_COMR0 = TM_COMR1 = 0x0; //timer target, ch 0 & 1
  TM_COMCR = 0x3; // reset on COMR, enable interrupt
  TM_OCR = 0x36; // toggle TM_COM0/COM1 on match

  TM_CTRL = 0x3; // use PCLK, reset timer, enable timer
  TM_CTRL = 0x1; // use PCLK, start timer, enable timer

  // MUX - select TIMER IOs
  MUXC_SELECT = 0x10;
  */
  
  pinMode_input(26);
  pinMode_input(27);

  // Define variables
	short dspsent_i = 0;	//keeps track of bits sent to DSP core
	short dec_i = 0; 		//keeps track of decoded bits
  short recv_i = 0;		//keeps track of received channel bits 
  char temp_recv_i = 0;
	
	accum LLR_Le_arr[256]; //stores LLR / Le per half iteration
	accum noise_var;
	
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
  accum noisevar_mean;
  

  while(1){
    
    // ## GET_CHNL_DATA
    // Get data from channel - currently from uart (without noise!!)
		while(recv_done==0){
		  //recv and send back ACK
		  char temp_uart;
  		//accum_int_t xtemp;
  		
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

          // send sbit to dsp for aiding in noise estimation, process is sequential
          //kickstart with a header, wait for ACK
          if(recv_i==1){
            while(noc_IC_txbuff_isfull==1)__NOP(); IC_NOC_TX_BUFF3 = IO_NOVAR_HDR;
            while(noc_IC_rxbuff3_av!=1)__NOP(); int temp_ack = IC_NOC_RX_BUFF3;
            noisevar_mean = 0;
          }
          //send sbit
          while(noc_IC_txbuff_isfull==1)__NOP();
          temp_accumint.accum_cont = recv_sbit[recv_i-1];
          IC_NOC_TX_BUFF3 = temp_accumint.int_cont;

          //sum of sbit, for noise var estimation
          noisevar_mean += recv_sbit[recv_i-1];
  			}
  		}
  		
			//increment received bit count, mark if a complete frame is received
			if(recv_i==256){
        // set flag
				recv_done = 1;

        //calculate noise variance estimate
        // -- E{x^2} part
        while(noc_IC_rxbuff3_av!=1)__NOP();
        temp_accumint.int_cont = IC_NOC_RX_BUFF3;
        noise_var = temp_accumint.accum_cont >> 8; //divide by K

        // -- E{|x|}^2 part
        noisevar_mean = noisevar_mean >> 8; //divide by K
        while(noc_IC_txbuff_isfull==1)__NOP();
        temp_accumint.accum_cont = noisevar_mean;
        IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //send to DSP for squaring
        while(noc_IC_rxbuff3_av!=1)__NOP();
        temp_accumint.int_cont = IC_NOC_RX_BUFF3;
        noisevar_mean = temp_accumint.accum_cont;

        noise_var -= noisevar_mean;
        noise_var = 1/noise_var;
			}
		}
    
    // ## DTRMN_PKT_TYPE
    // Examine packet header
    if( ((IC_NOC_CSR0 & 0x40)==0x40) && (flag_pkt_type==0) )
      flag_pkt_type = IC_NOC_RX_BUFF3;
    
    /*
    // ~~DEBUG
    if( (IC_NOC_CSR0 & 0x4)==0x4 ){
      char tempbuff = IC_NOC_RX_BUFF1;
      _putchar(tempbuff);
      while( !_getchar(&tempbuff) )__NOP(); //wait for ACK
      while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF1 = tempbuff;
    }
    */
    
    // ## RECV_LLR_NC
    // Receive LLR / Le from NC1 core accordingly
		//if( ((IC_NOC_CSR0 & 0x10)==0x10) && (((IC_NOC_CSR2&0xf000000)>>24)>=0x8) ){
		if( (IC_NOC_CSR0 & 0x10)==0x10 ){	
		  //adjust index accordingly, depends on dec1 / dec2
			//for(char i=0; i<8; i++){
  			unsigned char llridx;
  			if(dec_stat==0)
  				llridx = dec_i;
  			else
  				llridx = I_last_recv = drp_idxcalc(r_K, r_M, I_last_recv, (unsigned char*)intrlv_P, dec_i);
  			
  			//store LLR / Le
  			temp_accumint.int_cont = IC_NOC_RX_BUFF2;
  			LLR_Le_arr[llridx] = temp_accumint.accum_cont;
  			
  			dec_i++;
			//}
			
		  //send back ACK per x flits
		  //while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF2 = IO_LLRACK_HDR; // Header - LLR ack
		  while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF2 = dec_i;
      
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
		// Send bits to DSP core
		
		// * check for ACK per 1 bits packet sent
		if( (flag_pkt_type==DSP_BITSACK_HDR) && ((IC_NOC_CSR0 & 0x40)==0x40) ){
		  flag_pkt_type = 0; //clear packet type's flag
		  int temp_ack = IC_NOC_RX_BUFF3;
		  
		  dspsend_hold = 0;
		  
			//dspsend_hold criteria #2- dspsent_i is a multiple of r_win
			//hold sending if it sent out two windows, and none of them returned completely
			//or if it has sent all bits of a frame
		  if( dspsent_i==256 || ((dspsent_i&(r_win-1))== 0 && ((dspsent_i>>r_win_mul)-(dec_i>>r_win_mul))>=2) )
				dspsend_hold = 2;
		}
		
		// * sending part
		if( (recv_i>dspsent_i) && (dspsend_hold==0) ){
			// ** Send control bits, if this is first bit of an iteration
			if(dspsent_i==0){
			  //send calcLLR and noise_var to DSP core
			  while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF3 = IO_CHNLCTRL_HDR; // Header - chnl_control
			  while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF3 = calcLLR;
			  while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = noise_var; IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
			  
        //send calcLLR to NC1
			  while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF2 = calcLLR;
        
			  //wait for ACK -  since it is blocking, no need to differentiate this one
			  while((IC_NOC_CSR0 & 0x40)!=0x40)__NOP();
			  int temp_ack = IC_NOC_RX_BUFF3;
			  
			}
			
			// ** Send data bits
			while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); IC_NOC_TX_BUFF3 = IO_BITS_HDR; // Header - bits
						
			//if dec1 is active
			if(dec_stat==0){
				while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = recv_sbit[dspsent_i]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //systematic bit
				while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = recv_pbit1[dspsent_i]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //parity bit
				
				//apriori prob - ln(apriori) and ln(1-apriori)
				if(halfitr_ctr==0){ 
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = -0.6931;
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
				}
				else{
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = ln_approx(LLR_Le_arr[dspsent_i]);
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = ln_approx(1-LLR_Le_arr[dspsent_i]);
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
				}
			}
			
			//if dec2 is active
			else{ 
				I_last_send = drp_idxcalc(r_K, r_M, I_last_send, (unsigned char*)intrlv_P, dspsent_i);
				while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = recv_sbit[I_last_send]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //systematic bit
				while((IC_NOC_CSR1 & 0x2)==0x2)__NOP(); temp_accumint.accum_cont = recv_pbit2[dspsent_i]; IC_NOC_TX_BUFF3 = temp_accumint.int_cont; //parity bit
				
				//apriori prob - ln(apriori) and ln(1-apriori)
				if(halfitr_ctr==0){
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = -0.6931;
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
				}
				else{
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = ln_approx(LLR_Le_arr[I_last_send]); 
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
					while((IC_NOC_CSR1 & 0x2)==0x2)__NOP();
					temp_accumint.accum_cont = ln_approx(1-LLR_Le_arr[I_last_send]); 
					IC_NOC_TX_BUFF3 = temp_accumint.int_cont;
			  }
			}
					  			
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


/*
// interrupt triggered - read bits from UART
void ioc_recv_uart(){
  static char idx = 0; //locally used to keep track on data
  char temp;
  if(idx==0){
    temp = _getchar();
    GPIO_BTGL = 0x1 << ledpin;
    recv_sbit[recv_i] =  temp; _putchar(temp);
    //recv_sbit[recv_i] =  (recv_sbit[recv_i]<<1)-1;
    idx++;
  }
  else if(idx==1){
    temp = _getchar();
		recv_pbit1[recv_i] = temp; _putchar(temp);
    //recv_pbit1[recv_i] = (recv_pbit1[recv_i]<<1)-1;
    idx++;
  }
  else if(idx==2){
    temp = _getchar();
		recv_pbit2[recv_i] = temp; _putchar(temp);
    //recv_pbit2[recv_i] = (recv_pbit2[recv_i]<<1)-1;
	  recv_i++;
	  idx = 0;
	}
	
}

*/

