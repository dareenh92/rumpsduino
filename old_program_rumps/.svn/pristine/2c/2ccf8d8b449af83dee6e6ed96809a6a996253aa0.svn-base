//------------------------------------------------------------------------------
//
// Main Program
// Application		: blinky
// Core						: Any
// Purpose
//	- blink an LED using corresponding core

#include "main.h"
const char RESETn = 26; // Lime hardware reset, GPIO7
const char SEN = 27; // Lime SPI slave enable, GPIO8

void write_lms6002_reg(uint8_t reg_addr, uint8_t data);
uint8_t read_lms6002_reg(uint8_t reg_addr);

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
  char ch;
  uint8_t reg_addr;
  uint8_t data_w;
  uint8_t data_r;
  
  //
  pinMode_output(RESETn);
  pinMode_output(SEN);
  digitalWrite_high(RESETn);
  digitalWrite_high(SEN);
  
   	  	 // CPHA, CPOL, BC,   IE,   FSB,  SS,   CR
  spi_init( 0x0,  0x0,  0x0,  0x0,  0x0,  0x0,  0x1);
  spi_enable();
  
  // mux
  MUXC_SELECT = 0x00000040;	// SPI selected
  MUXC_PU &= 0x0fffffff;	// SPI PU disabled
  
  uart_hd_init_uart(0x1);
  
  while(1){
  	// receive command from terminal
    ch = uart_hd_getchar();
  
    // acts by command
    if(ch=='a'){
      //default 0x32
      reg_addr = 0x05;
      data_w = 0x38;
    }
    else if(ch=='b'){
      //default 0x41
      reg_addr = 0x10;
      data_w = 0x43;
    }
    else if(ch=='r'){
      //hardware reset
      digitalWrite_low(RESETn);
      int i;
      for (i=0; i<1000; i++);
      digitalWrite_high(RESETn);
    }
    
    //uart_hd_println("#######");
    data_r = 0xA;
    data_r = read_lms6002_reg(reg_addr);
    //uart_hd_println("Read(before): " + String(data_r, HEX));
    uart_hd_putchar(data_r);
    
    if(ch!='r'){
      write_lms6002_reg(reg_addr, data_w);
      //uart_hd_println("Writing " + String(data_w, HEX));
    	uart_hd_putchar(data_w);
    	data_r = 0xA;
      data_r = read_lms6002_reg(reg_addr);
      //uart_hd_println("Read(after): " + String(data_r, HEX));
    	uart_hd_putchar(data_r);
    }
  }
   
	return 0;
}
  
void write_lms6002_reg(uint8_t reg_addr, uint8_t data){
  //takes 7 bits, add CMD=1 for write on MSB
  reg_addr = (reg_addr & 0x7f) | 0x80;
	
  //do transfer - in 16 bit mode
  //change to 16bit mode for this operation
  SPI_CTRL |= 0x00000010;
  
  digitalWrite_low(SEN);
  spi_send( (reg_addr << 8) | (data & 0xff) );
  while ((SPI_STATE & 0x2) == 0x2) // wait SP_TIP low
    __NOP();
  digitalWrite_high(SEN);
 
  //revert back to 8bit mode
  SPI_CTRL &= 0xffffffef;
  
}

uint8_t read_lms6002_reg(uint8_t reg_addr){
  uint8_t byte_read;
  
  //takes 7 bits, add CMD=0 for read on MSB
  reg_addr = (reg_addr & 0x7f);

  //do transfer
  digitalWrite_low(SEN);
  spi_send(reg_addr);
  byte_read = spi_recv();
  digitalWrite_high(SEN);

  return byte_read;
}

