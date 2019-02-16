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

//shared with main0/main.c
#define F_SAMPLERATE 8
#define H_SAMPLERATE 4
#define Q_SAMPLERATE 2
#define TIMING_PILOTPAIR 8
#define RX_STOP 0xa0
#define RX_DETECT 0xa1
#define RX_TIMING_SYNC 0xa2
#define RX_FREQ_SYNC 0xa3
#define RX_PAYLOAD 0xa4
#define RX_SENDUP 0xa5;

volatile uint8_t rx_state;
int codeword[768];
int tempdump[2], ted_dump[TIMING_PILOTPAIR], trigdump;

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
  static volatile int in_Idata = 0;
  static volatile int in_Qdata = 0;

  static volatile int trig_Ibuff[2] = {0};
  static volatile int trig_Qbuff[2] = {0};
  static volatile int8_t IQtrig = 0;

  //sampling point tracker
  static volatile int nextStep = Q_SAMPLERATE; // init nextStep count
  static volatile int stepCount = Q_SAMPLERATE-1; // init to trigger 1st time
  static volatile int nSentSamp = 0;
  static volatile unsigned int univ_ctr = 0; // general reusable counter

  if((TM_IF & 0x1) == 0x1){

    // Read data - I and Q
    in_Idata = (GPIO_DATAIN>>12) & 0xfff;
    in_Idata = (in_Idata << 20) >> 20; // integer signed extension
    GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (testbuff & 0xfff);
    testbuff =  ~testbuff; 

    asm("push {r0}\n"
        "movs  r0, #7\n"      // 1 cycle
        "loop: sub  r0, r0, #1\n" // 1 cycle
        "cmp r0, #0\n"         // 1 cycle
        "bne  loop\n"          // 2 if taken, 1 otherwise
        "pop {r0}\n");

    in_Qdata = (GPIO_DATAIN>>12) & 0xfff;
    in_Qdata = (in_Qdata << 20) >> 20; // integer signed extension
    GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (testbuff & 0xfff);
    testbuff =  ~testbuff; 

    // Detect TX signal
    if(rx_state==RX_DETECT){
      if(++stepCount==nextStep){
        stepCount = 0;

        trig_Ibuff[univ_ctr] = in_Idata;
        trig_Qbuff[univ_ctr] = in_Qdata;

        if(++univ_ctr==2){ // detect slope (sign change)
          univ_ctr = 1;
          int diff_i = abs_f(trig_Ibuff[0] - trig_Ibuff[1]);
          int diff_q = abs_f(trig_Qbuff[0] - trig_Qbuff[1]);

          if( (sign_f(trig_Ibuff[0]) != sign_f(trig_Ibuff[1])) &&
              (diff_i > 200) ){
            IQtrig = 1;
            tempdump[0] = trig_Ibuff[0];
            tempdump[1] = trig_Ibuff[1];
          }
          else if( (sign_f(trig_Qbuff[0]) != sign_f(trig_Qbuff[1])) &&
                   (diff_q > 200) ){
            IQtrig = 2;
            tempdump[0] = trig_Qbuff[0];
            tempdump[1] = trig_Qbuff[1];
          }

          trig_Ibuff[0] = trig_Ibuff[1];
          trig_Qbuff[0] = trig_Qbuff[1];
        } 
        
        if(IQtrig>0){
          trigdump = IQtrig;
          nextStep = F_SAMPLERATE+H_SAMPLERATE; // next TIMINGPILOT pair
          rx_state = RX_TIMING_SYNC;
        }
      }
    }

    // Timing offset correction
    else if(rx_state==RX_TIMING_SYNC){
      if(++stepCount==nextStep){
        //send I Q to DSP core for synchronization
        while(noc_IC_txbuff_isfull==1)__NOP(); IC_NOC_TX_BUFF3 = in_Idata;
        while(noc_IC_txbuff_isfull==1)__NOP(); IC_NOC_TX_BUFF3 = in_Qdata;

        //update sampling point
        nextStep = H_SAMPLERATE; // within 3 samples

        if(++nSentSamp==3){ // update next step per 3 samples
          nSentSamp = 0;
          
          //receive TED from DSP core
          while(noc_IC_rxbuff3_av!=1)__NOP();
          int ted = IC_NOC_RX_BUFF3;
          nextStep = (F_SAMPLERATE) - ted; // full step 
          ted_dump[univ_ctr] = ted;

          if(++univ_ctr==TIMING_PILOTPAIR){
            GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (0xfff);
            GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (0x000);
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
        if(IQtrig==1){
          codeword[univ_ctr] = in_Idata;
        }
        else if(IQtrig==2){
          codeword[univ_ctr] = in_Qdata;
        }
        if(univ_ctr==0){
          GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (0xfff);
          GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (0x000);
        }
        nextStep = F_SAMPLERATE;
        stepCount = 0;

        if(++univ_ctr==768){ //back to init state
          IQtrig = 0;
          univ_ctr = 0;
          nextStep = H_SAMPLERATE;
          stepCount = H_SAMPLERATE-1;
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
