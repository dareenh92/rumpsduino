//------------------------------------------------------------------------------
//
// Main Program
// Application    : softboot_uart
// Core            : Any
// Purpose
//  - receive program data byte per byte through uart, and send them back for checking purpose
//  - flash those bytes into each core

#include "main.h"

#define app_4k_address 4096
#define app_8k_address 8192

#define UART_115200 0x1		// 0x0 => 203400, 0x1 => 115200


void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
  volatile unsigned char ch;

  volatile unsigned short turn_count = 0;
  volatile unsigned int prog_byte_count = 0;

  volatile unsigned short program_start = 0;

  volatile char core_byte[4];

  //enable write to the flash
  FLASH_KEY = 0xc6a5;
  
  //initialize uart
  uart_hd_init_uart();

  //main loop
  while(1)
  {
    //ch = _getchar();
    //_putchar(ch);
	//uart_hd_putchar(0x55); //
	//uart_hd_putchar(0x00); //
	//uart_hd_putchar(0x7e); //

    //** Bytes storing part
    if(program_start == 0){
      core_byte[turn_count] = uart_hd_getchar(UART_115200); //get byte
      uart_hd_putchar(core_byte[turn_count], UART_115200); //return byte for checking

      turn_count++;

      // after 4 bytes, start program flashing
      if (turn_count == 4){
        program_start = 1;
      }
    }

    //* Program flashing part
    else{

      //GPIO_DATAOUT |= 0x0000000f;
      //GPIO_DATAOUT &= 0xfffffffe; //CHECKPOINT
      
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
        
        //GPIO_DATAOUT |= 0x0000000f;
        //GPIO_DATAOUT &= 0xfffffffd; //CHECKPOINT

        //wait for erase_finish flag
        while(FLASH_IF != 0x1) __NOP();
      }
      
      FLASH_PROGADDR  = app_4k_address+prog_byte_count;
      FLASH_PROGDATA  = core_byte[0];
      FLASH_PROG      = 0x1;
      
      //GPIO_DATAOUT |= 0x0000000f;
      //GPIO_DATAOUT &= 0xfffffffb; //CHECKPOINT

      //wait for program_finish flag
      while(FLASH_IF != 0x1) __NOP();

      //increase byte count
      prog_byte_count++;
      
      /////////////////  
      //wait for corresponding ACK by polling
      //while( (IC_NOC_BD_CSR & 0x8)!=0x8 && (IC_NOC_BD_CSR & 0x20)!=0x20 && (IC_NOC_BD_CSR & 0x80)!=0x80 )
      while( (IC_NOC_BD_CSR & 0xa8)!=0xa8 )
        __NOP();
      
      //reset those ACK bits
      IC_NOC_BD_CSR = 0x00000a8;
      
      //reset counter and flag for next iteration
      turn_count = 0;
      program_start = 0;
      
      //send back checksum for each successfull flash
      volatile char chk_sum = core_byte[0] + core_byte[1] + core_byte[2] + core_byte[3];
      chk_sum = ~(chk_sum) + 0x1;
      uart_hd_putchar('U', UART_115200);
    }


    //int i;
    //for(i=0; i<100000; i++) idle();

  }// end of main loop
   
  return 0;
}
  

