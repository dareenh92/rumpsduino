//------------------------------------------------------------------------------
//

#include "main.h"

// cannot and should not use printf(), data sharing problem
// message() should be reentrant (by default) 

unsigned int systick_cnt = 0;
unsigned int ext_0_cnt = 0;
unsigned int ext_1_cnt = 0;
unsigned int ext_2_cnt = 0;
unsigned int ext_3_cnt = 0;
unsigned int ext_4_cnt = 0;
unsigned int ext_5_cnt = 0;
unsigned int ext_6_cnt = 0;
unsigned int ext_7_cnt = 0;
unsigned int ext_8_cnt = 0;
unsigned int ext_9_cnt = 0;
unsigned int ext_10_cnt = 0;
unsigned int ext_11_cnt = 0;
unsigned int ext_12_cnt = 0;
unsigned int ext_13_cnt = 0;
unsigned int ext_14_cnt = 0;
unsigned int ext_15_cnt = 0;

#define FULL_SAMPLERATE 8
#define HALF_SAMPLERATE 4
#define TIMING_PILOTPAIR 4

//shared with main0/main.c
#define RX_STOP 0xa0
#define RX_DETECT 0xa1
#define RX_TIMING_SYNC 0xa2
#define RX_FREQ_SYNC 0xa3
#define RX_PAYLOAD 0xa4
#define RX_SENDUP 0xa5;

volatile uint8_t rx_state;
int codeword[768];
int tempdump, ted_dump;

//-----------------------------------------------------------------------------------
void NMI_Handler(void)
{
  int i;
  
  __SEV();
  
  //write32(0x200, 0x11111111);
    //while(1);
    for (i = 0; i < 2000; i ++);
    __SEV();
}

//-----------------------------------------------------------------------------------
void HARD_FAULT_Handler(void)
{
  write32(0x200, 0x22222222);
  while(1);
}

//-----------------------------------------------------------------------------------
void SYSTICK_Handler(void)
{
  __SEV();
    systick_cnt++;
    write32(0x200, 0x33333333);
  while(1);
}

//-----------------------------------------------------------------------------------
void EXTERNAL_0_Handler(void)
{
  int i;
  
  __SEV();
  ext_0_cnt++;
  NVIC_ClearPendingIRQ(0);
  
  for (i = 0; i < 200; i ++);
  
  __SEV();
}

//-----------------------------------------------------------------------------------
void EXTERNAL_1_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(1);
    ext_1_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_2_Handler(void) // pp_rx
{
  __SEV();
  
  NVIC_DisableIRQ(2);     // disable PP_RX interrupt until the data processed

  //NVIC_ClearPendingIRQ(2);
  ext_2_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_3_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(3);
    ext_3_cnt++;
}

//-----------------------------------------------------------------------------------
// NoC RX Int
void EXTERNAL_4_Handler(void)
{
  unsigned int data;
  
  __SEV();
  
  NVIC_DisableIRQ(4);     // disable NOC RX interrupt until the data processed

  //NVIC_ClearPendingIRQ(4);
    ext_4_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_5_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(5);
    ext_5_cnt++;
}

//-----------------------------------------------------------------------------------
//TIMER
void EXTERNAL_6_Handler(void)
{
  __SEV();

  static volatile int testbuff = 0xfff;

  //signal variable
  static volatile int in_temp = 0;
  static volatile int in_Idata = 0;
  static volatile int in_Qdata = 0;

  //sampling point tracker
  static volatile int nextStep = HALF_SAMPLERATE; // init nextStep count
  static volatile int stepCount = HALF_SAMPLERATE-1; // init to trigger 1st time
  static volatile int nSentSamp = 0;
  static volatile unsigned int univ_ctr = 0; // general reusable counter

  if((TM_IF & 0x1) == 0x1){

    // Read data - general
    in_temp = (GPIO_DATAIN>>12) & 0xfff;
    in_temp = (in_temp << 20) >> 20; // integer signed extension

    // Detect TX signal
    if(rx_state==RX_DETECT){
      if( (in_temp>650) || (in_temp<-650) ){
        tempdump = in_temp;
        rx_state = RX_TIMING_SYNC;
      }
    }

    // Timing offset correction
    if(rx_state==RX_TIMING_SYNC){
      if(++stepCount==nextStep){
        //take I
        in_Idata = in_temp;
        //GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (testbuff & 0xfff);
        //testbuff =  ~testbuff; 
        
        asm("push {r0}\n"
            "movs  r0, #7\n"      // 1 cycle
            "loop: sub  r0, r0, #1\n" // 1 cycle
            "cmp r0, #0\n"         // 1 cycle
            "bne  loop\n"          // 2 if taken, 1 otherwise
            "pop {r0}\n");        
        
        //take Q
        //in_Qdata = (GPIO_DATAIN>>12) & 0xfff;
        //GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (testbuff & 0xfff);
        //testbuff =  ~testbuff; 
        
        //send I Q to DSP core for synchronization
        while(noc_IC_txbuff_isfull==1)__NOP(); IC_NOC_TX_BUFF3 = in_Idata;
        //while(noc_IC_txbuff_isfull==1)__NOP(); IC_NOC_TX_BUFF3 = in_Qdata;

        //update sampling point
        nextStep = HALF_SAMPLERATE; // within 3 samples

        if(++nSentSamp==3){ // update next step per 3 samples
          nSentSamp = 0;
          
          while(noc_IC_rxbuff3_av==0)__NOP();
          int ted = IC_NOC_RX_BUFF3;
          nextStep = (FULL_SAMPLERATE) - ted; // full step 
          ted_dump = ted;

          univ_ctr++;
          if(univ_ctr==TIMING_PILOTPAIR){
            univ_ctr = 0;
            rx_state = RX_PAYLOAD; // should be RX_FREQ_SYNC later
          }
        }
        
        stepCount = 0; //reset stepCount
      }

    }

    // Frequency offset correction
    else if(rx_state==RX_FREQ_SYNC){
      __NOP();
      //rx_state = RX_PAYLOAD;
    }

    // Retrieve payload
    else if(rx_state==RX_PAYLOAD){

      if(++stepCount==nextStep){
        //GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (testbuff & 0xfff);
        //testbuff =  ~testbuff; 
        codeword[univ_ctr++] = in_temp;
        
        nextStep = FULL_SAMPLERATE;
        stepCount = 0;

        if(univ_ctr==768){ //back to init state
          univ_ctr = 0;
          nextStep = HALF_SAMPLERATE;
          stepCount = HALF_SAMPLERATE-1;
          rx_state = RX_SENDUP;
          NVIC_DisableIRQ(6);
        }

      }
    }

    TM_IF = 0x0;

  }  

  NVIC_ClearPendingIRQ(6);
    ext_6_cnt++;  

}

//-----------------------------------------------------------------------------------
void EXTERNAL_7_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(7);
    ext_7_cnt++;
}

//void ioc_recv_uart();

//-----------------------------------------------------------------------------------
void EXTERNAL_8_Handler(void)
{
  __SEV();
  
  NVIC_ClearPendingIRQ(8);
    ext_8_cnt++;
    
  //ioc_recv_uart();
  //GPIO_IF = 0x1;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_9_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(9);
    ext_9_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_10_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(10);
    ext_10_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_11_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(11);
    ext_11_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_12_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(12);
    ext_12_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_13_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(13);
    ext_13_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_14_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(14);
    ext_14_cnt++;
}

//-----------------------------------------------------------------------------------
void EXTERNAL_15_Handler(void)
{
  __SEV();

  NVIC_ClearPendingIRQ(15);
    ext_15_cnt++;
}
