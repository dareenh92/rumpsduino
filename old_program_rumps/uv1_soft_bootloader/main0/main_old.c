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

volatile unsigned int turn_count = 0;
volatile unsigned int prog_byte_count = 0;

volatile unsigned int program_start = 0;

volatile unsigned int core_byte[4][4];

void main_scheduler (void);

#define app_4k_address 4096
#define app_8k_address 8192

//------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
	
	unsigned int sleep_count = 0;
	
	//NVIC_SetPriority(2, 0);		// set Ext INT 2 (pp rx) priority
  NVIC_SetPriority(5, 0);   // set Ext INT 4 (noc bd) priority 
  
	//NVIC_EnableIRQ(2);			  // enable Ext INT 2 (pp rx) 
  NVIC_EnableIRQ(5);        // enable Ext INT 4 (noc bd) 
	
  MUXC_SELECT = 0x2f; // set mux controller to enable parallel port IO
  
  //PP_ST = 0x00000600;   // SET PP_RX_IE and PP_RX_IM
  
  while(1)
  {
    // -------------------------------------------------------------------------------------
    // check availability of pp data
    // -------------------------------------------------------------------------------------
    if ((PP_ST & 0x4) == 0x4 && program_start == 0)
    {
      if (turn_count == 4){
    	  turn_count = 0;
        program_start = 1;
      }
      else{
      	core_byte[turn_count][0] = PP_BUFF;
      	core_byte[turn_count][1] = PP_BUFF >> 8;
      	core_byte[turn_count][2] = PP_BUFF >> 16;
      	core_byte[turn_count][3] = PP_BUFF >> 24;
      
        turn_count++;
      }
    }
    
    // write program to other cores and itself
    if (program_start == 1)
    {
      for (i = 0; i < 4; i++)
      {
        IC_NOC_TX_BD_BUFF1 = core_byte[1][i];
        IC_NOC_TX_BD_BUFF2 = core_byte[2][i];
        IC_NOC_TX_BD_BUFF3 = core_byte[3][i];
      
        //---
        if (prog_byte_count % 512 == 0){
          FLASH_ERASCTR = (prog_byte_count+app_4k_address)/512;
          FLASH_ERA     = 0x1;
        }
          
        FLASH_PROGADDR  = prog_byte_count+app_4k_address;
        FLASH_PROGDATA  = core_byte[0][i];
        FLASH_PROG      = 0x1;
          
        prog_byte_count++;
      }
      
      program_start = 0;
    }
  }
   
	return 0;
}
  

