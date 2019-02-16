//------------------------------------------------------------------------------
//
// Main Program
// Application    : lms6002d_txtest
// Core           : Any
// Purpose
//  - Configuring LMS6002D registers for TX test

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
  MUXC_SELECT = 0x00000040; // SPI selected
  MUXC_PU &= 0x0fffffff;  // SPI PU disabled
  
  uart_hd_init_uart(0x1);
  
  while(1){
    // receive command from terminal
    ch = uart_hd_getchar();
    
    if(ch=='s'){
      //####
      // TOP Level
      //####
      // Soft tx enable
      reg_addr = 0x05; data_w = 0x3a;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);

      // Clock buffers - Tx DSM SPI
      reg_addr = 0x09; data_w = 0x41;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);
      
      //####
      // Tx LPF
      //####
      // select LPF bandwidth (.75MHz)
      reg_addr = 0x34; data_w = 0x3e;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);
      
      //####
      // Tx RF
      //####
      // VGA1 gain (-10dB)
      reg_addr = 0x41; data_w = 0x19;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);
      
      // VGA2 gain (15dB)
      reg_addr = 0x45; data_w = 0x78;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);
      
      // Select PA1
      reg_addr = 0x44; data_w = 0x0b;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);
      
      //####
      // Tx PLL + DSM
      //####
      // Output frequency
      reg_addr = 0x15; data_w = 0xfd;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);
      
      // CP current (1200uA)
      reg_addr = 0x16; data_w = 0x8c;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);
      
      // CP UP offset current (30uA)
      reg_addr = 0x17; data_w = 0xe3;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);
      
      // N integer 
      reg_addr = 0x10; data_w = 0x54;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);

      // N fractional over 3 registers
      reg_addr = 0x11; data_w = 0x66;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);

      reg_addr = 0x12; data_w = 0x66;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);

      reg_addr = 0x13; data_w = 0x66;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      if(data_r==data_w)
        uart_hd_putchar(data_r);
      else
        uart_hd_putchar(0xff);

    }
    
    else if(ch=='r'){
      //hardware reset
      digitalWrite_low(RESETn);
      for (int i=0; i<1000; i++)__NOP();
      digitalWrite_high(RESETn);
    }

    else if(ch=='v'){ // auto vco cap tuning
      uint8_t last_vtune = 0x2;
      uint8_t curr_vtune;
      uint8_t cmax, cmin;

      for(uint8_t i=0; i<64; i++){
        // change vcocap
        write_lms6002_reg(0x19, i + 0x80);
        for(uint8_t delay=0; delay<100; delay++)__NOP();
        
        // read vtune
        curr_vtune = read_lms6002_reg(0x1a)>>6;
        
        // find cmin cmax
        if(last_vtune==0x2 && curr_vtune==0x0)
          cmin = i;
        else if(last_vtune==0x0 && curr_vtune==0x1){
          cmax = i;
          write_lms6002_reg(0x19, ((cmin+cmax)/2) + 0x80);
          break;
        }
        last_vtune = curr_vtune;
      }
      uart_hd_putchar(cmin);
      uart_hd_putchar(cmax);

    }


    else if(ch=='c'){ // calibrations - in sequence
      //####
      // (1) DC offset calibration of LPF Tuning Module
      //####      
      uint8_t topspi_clken;

      topspi_clken = read_lms6002_reg(0x09); // save CLK_EN[5]
      write_lms6002_reg(0x09, topspi_clken|=0x20); // CLK_EN[5]=1

      //-----------
      // General DC Calibration - part of (1)
      uint8_t clbr_result = 0; // 0 'false', 1 'true'

      write_lms6002_reg(0x03, 0x08); // DC_ADDR = 0
      write_lms6002_reg(0x03, 0x28); // DC_START_CLBR = 1 
      write_lms6002_reg(0x03, 0x08); // DC_START_CLBR = 0
      
      if(read_lms6002_reg(0x00)==31){ // read DC_REGVAL
        
        write_lms6002_reg(0x00, 0x0); // DC_REG_VAL = 0;
        write_lms6002_reg(0x03, 0x28); // DC_START_CLBR = 1 
        write_lms6002_reg(0x03, 0x08); // DC_START_CLBR = 0
        
        if(read_lms6002_reg(0x00)!=0) // read DC_REGVAL
          clbr_result = 1;
      }
      else
        clbr_result = 1;

      uart_hd_putchar(clbr_result);

      // End of general DC calibration
      //-----------

      // check general DC calibration result
      if(clbr_result==0){ // PANIC: algo doesnt converge
        uart_hd_putchar(0xff);
      }
      else if(clbr_result==1){ // SUCCESS: algo converge
        uart_hd_putchar(0x11);
        
        data_r = read_lms6002_reg(0x00); // DCCAL = DC_REGVAL
        //write_lms6002_reg(0x55, data_r) // RXLPF::DCO_DACCAL = DCCAL
        write_lms6002_reg(0x35, data_r); // TXLPF::DCO_DACCAL = DCCAL
        write_lms6002_reg(0x09, topspi_clken); // restore CLK_EN[5]
      }

      //####
      // (2) LPF Bandwidth Tuning procedure
      //####

      //------------
      // PLL ref clock is not 40MHz, so do these

      // power down txvga2 (opt)
      
      // enable TxPLL and set to produce 320MHz
      
      // use 40MHz from TxPLL, TOP::CLKSEL_LPFCAL = 0

      // power up LPF tuning clock genblock, TOP::PD_CLKLPFCAL = 0

      //-------------
      // Common procedures





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

