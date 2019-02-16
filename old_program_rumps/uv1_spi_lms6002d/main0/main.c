//------------------------------------------------------------------------------
//
// Main Program
// Application		: spi_lms6002d test
// Core						: Any
// Purpose
//	- Test of controlling Lime LMS6002D register

#include "main.h"
const char RESETn = 26; // Lime hardware reset, GPIO7

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
  digitalWrite_high(RESETn);
  
   	  	 // CPHA, CPOL, BC,   IE,   FSB,  SS,   CR
  spi_init( 0x0,  0x0,  0x1,  0x0,  0x0,  0x1,  0x1);
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
      for (i=0; i<1000; i++)__NOP();
      digitalWrite_high(RESETn);
    }
    
    //uart_hd_println("#######");
    //before write
    data_r = read_lms6002_reg(reg_addr);
    uart_hd_putchar(data_r);
    
    if(ch!='r'){
    	//data write
      write_lms6002_reg(reg_addr, data_w);
    	uart_hd_putchar(data_w);
    	//after write
    	int i;
    	for (i=0; i<1000; i++)__NOP();
 			data_r = read_lms6002_reg(reg_addr);
    	uart_hd_putchar(data_r);
    }
  }
   
	return 0;
}
  
void write_lms6002_reg(uint8_t reg_addr, uint8_t data){
  //takes 7 bits, add CMD=1 for write on MSB
  reg_addr = (reg_addr & 0x7f) | 0x80;
	  
  spi_send( (reg_addr << 8) | (data & 0xff) );
}

uint8_t read_lms6002_reg(uint8_t reg_addr){
  //takes 7 bits, add CMD=0 for read on MSB
  reg_addr = (reg_addr & 0x7f);

  //do transfer
 	spi_send((reg_addr << 8));
  
  while ((SPI_STATE & 0x1) == 0x0) // wait SP_IF high
    __NOP();
	
  SPI_STATE |= 0x1; // write 1 to clear
  
  return (SPI_DATA & 0xff);
}

