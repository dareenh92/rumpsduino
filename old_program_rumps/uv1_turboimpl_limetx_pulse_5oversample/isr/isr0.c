//------------------------------------------------------------------------------
//

#include "main.h"

// cannot and should not use printf(), data sharing problem
// message() should be reentrant (by default) 

#define RCOS_FIRLEN 25

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
volatile uint8_t txd_ready;
unsigned char codeword[768];

//**for pulse shaping
int rcos_lut[2][25]; // raised cosine impulse response LUT
int arrctr0, arrctr1, arrctr2, arrctr3, arrctr4; // pulse components counter
unsigned int symctr0, symctr1, symctr2, symctr3, symctr4; // symbol counter
volatile int send_buff;


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
  static volatile unsigned int txd_sendctr = 0;
  static volatile unsigned int tempctr = 0;
  static volatile int firstcall = 0; // sync on first INT_ENABLE

  if((TM_IF & 0x1) == 0x1){

    if( firstcall!=0 && txd_ready==1 ){
      
      /*
      // Without pulse shaping
      if(codeword[txd_sendctr]==0)
        GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | 0x800;
      else
        GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | 0x7ff;
      if(++tempctr==5){
        tempctr = 0;
        if(++txd_sendctr==768)
          txd_sendctr = 0;
      }
      */

      GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (testbuff & 0xfff);
      testbuff =  ~testbuff; 
      // With pulse shaping
      //GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000) | (send_buff & 0xfff);
      
      arrctr0++;
      if(arrctr0 == 25){
        arrctr0 = 0;
        symctr0 += 5;
      }

      arrctr1++;
      if(arrctr1 == 25){
        arrctr1 = 0;
        symctr1 +=5;
      }

      arrctr2++;
      if(arrctr2 == 25){
        arrctr2 = 0;
        symctr2 += 5;
      }

      arrctr3++;
      if(arrctr3 == 25){
        arrctr3 = 0;
        symctr3 += 5; 
      }

      arrctr4++;
      if(arrctr4 == 25){
        arrctr4 = 0;
        symctr4 += 5;
      }

      if(symctr0>767 && symctr1>767 && symctr2>767 && 
         symctr3>767 && symctr4>767){
        //txd_sendctr = 0;

        arrctr0 = 0;
        arrctr1 = -5;
        arrctr2 = -10;
        arrctr3 = -15;
        arrctr4 = -20;

        symctr0 = 0;
        symctr1 = 1;
        symctr2 = 2;
        symctr3 = 3;
        symctr4 = 4;

        send_buff = rcos_lut[codeword[symctr0]][arrctr0];

        testbuff = 0xfff;

        txd_sendctr = txd_ready = firstcall = 0;
        NVIC_DisableIRQ(6); // disable this timer interrupt
      }
      
      send_buff = 0;

      if((arrctr0>=0) && (symctr0<768))
        send_buff += rcos_lut[codeword[symctr0]][arrctr0];

      if((arrctr1>=0) && (symctr1<768))
        send_buff += rcos_lut[codeword[symctr1]][arrctr1];

      if((arrctr2>=0) && (symctr2<768))
        send_buff += rcos_lut[codeword[symctr2]][arrctr2];

      if((arrctr3>=0) && (symctr3<768))
        send_buff += rcos_lut[codeword[symctr3]][arrctr3];

      if((arrctr4>=0) && (symctr4<768))
        send_buff += rcos_lut[codeword[symctr4]][arrctr4];
      
    }
    
    else{
      firstcall = 1;
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
