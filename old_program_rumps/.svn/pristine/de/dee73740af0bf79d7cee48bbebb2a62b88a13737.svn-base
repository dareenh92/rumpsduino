//------------------------------------------------------------------------------
//
// Main Program
// Application		: soft_bootloader
// Core						: Special Core
// Purpose
//	- software bootloading program
//	- special core will accept 4 programs, 1 for each core through
//		parallel port, and write it to its own and other three's flash

#include "main.h"
#include "uart.h"

void main_scheduler (void);

#define app_4k_address 4096
#define app_8k_address 8192

#define FLASH_4K_READ		((*(volatile unsigned char *)	(0x00001000)))

void blink_led1(int rep);
void blink_led2(int rep);
void blink_led3(int rep);

//------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
	
	volatile unsigned short turn_count = 0;
	volatile unsigned int prog_byte_count = 0;

	volatile unsigned short program_start = 0;

	volatile unsigned char core_byte[4];
	
	unsigned int sleep_count = 0;
	
	//NVIC_SetPriority(2, 0);		// set Ext INT 2 (pp rx) priority
  //NVIC_SetPriority(5, 0);   // set Ext INT 4 (noc bd) priority 
  
	//NVIC_EnableIRQ(2);			  // enable Ext INT 2 (pp rx) 
  //NVIC_EnableIRQ(5);        // enable Ext INT 4 (noc bd) 
	
  MUXC_SELECT = 0x01; // set mux controller to parallel port 0-7
  
  //PP_ST = 0x00000600;   // SET PP_RX_IE and PP_RX_IM
  
  GPIO_OEN_SET = 0xffff00;	// set GPIO [31:8]
  
  FLASH_KEY = 0xc6a5;
  
  while(1)
  {
    // -------------------------------------------------------------------------------------
    // check availability of pp data
    // -------------------------------------------------------------------------------------
    if ( ((PP_ST & 0x4) == 0x4) && (program_start == 0) )
    {
      if (turn_count == 3){
        program_start = 1;
      }
     	//store program into buffer
      //
      //since we only use 8bits Parallel port,
      //so we can only send 1 core's program per transfer
      //so 1st transfer is for core 0, 2nd is for core 1, and so on
      	
      core_byte[turn_count++] = PP_BUFF;
      
      //blink_led3();
    }
    
    // write program to other cores and itself
    if (program_start == 1)
    {
      //reset turn count for next iteration
    	turn_count = 0;
      
      //////////////////
      //to other cores
      IC_NOC_TX_BD_BUFF1 = core_byte[1];
      IC_NOC_TX_BD_BUFF2 = core_byte[2];
      IC_NOC_TX_BD_BUFF3 = core_byte[3];
      
      /////////////////
      //to itself
      if ((prog_byte_count % 512) == 0){
        FLASH_ERASCTR = (app_4k_address+prog_byte_count)/512;
        FLASH_ERA     = 0x1;
        
      	//wait for erase_finish flag
      	while(FLASH_IF != 0x1) __NOP();
      }
      
      FLASH_PROGADDR  = app_4k_address+prog_byte_count;
      FLASH_PROGDATA  = core_byte[0];
      FLASH_PROG      = 0x1;
      
      //wait for program_finish flag
      while(FLASH_IF != 0x1) __NOP();
      
      //blink_led1();
      
      
      /*
      unsigned char * ptr;
			unsigned char data;
      //read from 4kB address, per byte
			ptr = (volatile unsigned char *) (&FLASH_4K_READ +prog_byte_count);
			data = *ptr;
      //test data by blinking led
      blink_led1((data>>4) & 0xf);
      blink_led2(data & 0xf);
      */
      
      //increase byte count
      prog_byte_count++;
      
      /////////////////  
      //wait for corresponding ACK by polling
			//while( (IC_NOC_BD_CSR & 0x8)!=0x8 && (IC_NOC_BD_CSR & 0x20)!=0x20 && (IC_NOC_BD_CSR & 0x80)!=0x80 )
			while( (IC_NOC_BD_CSR & 0xa8)!=0xa8 )	
				__NOP();
			
      //blink_led2();
      
			//reset those ACK bits
			IC_NOC_BD_CSR = 0x00000a8;
      
      
      program_start = 0;
    }
  }
   
	return 0;
}
  
void blink_led1(int rep){
	int i = 0;
	int j = 0;
	
	for (i = 0; i < rep; i++){
  	GPIO_DATAOUT = 0x0100;
		for (j = 0; j < 1000000; j++)
			__NOP();
				
		GPIO_DATAOUT = 0x0000;
		for (j = 0; j < 1000000; j++)
			__NOP();
	}
}

void blink_led2(int rep){
	int i = 0;
	int j = 0;
	
  for (i = 0; i < rep; i++){
  	GPIO_DATAOUT = 0x0200;
		for (j = 0; j < 1000000; j++)
			__NOP();
				
		GPIO_DATAOUT = 0x0000;
		for (j = 0; j < 1000000; j++)
			__NOP();
	}
}

void blink_led3(int rep){
	int i = 0;
	int j = 0;
	
  for (i = 0; i < rep; i++){
  	GPIO_DATAOUT = 0x0400;
		for (j = 0; j < 1000000; j++)
			__NOP();
				
		GPIO_DATAOUT = 0x0000;
		for (j = 0; j < 1000000; j++)
			__NOP();
	}
}
