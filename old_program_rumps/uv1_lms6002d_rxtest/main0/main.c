//------------------------------------------------------------------------------
//
// Main Program
// Application    : lms6002d_rxtest
// Core           : Any
// Purpose
//  - Configuring LMS6002D registers for RX test

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
    
    if(ch=='s'){ // set LO freq and gain
      //####
      // TOP Level
      //####
      // Soft tx/rx enable
      reg_addr = 0x05; data_w = 0x36;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      // Clock buffers - Tx/Rx DSM SPI
      // RXout/ADC in switch "closed"
      reg_addr = 0x09; data_w = 0xc4;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      //####
      // Rx LPF
      //####
      // select LPF bandwidth (7MHz)
      reg_addr = 0x54; data_w = 0x0a;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      //####
      // Rx VGA2
      //####
      // VGA2 gain (3dB)
      reg_addr = 0x65; data_w = 0x01;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      //####
      // Rx FE
      //####
      // active LNA=LNA1, LNA gain mode=max gain
      reg_addr = 0x75; data_w = 0xd0;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      // VGA1 control feedback resistor (120)
      reg_addr = 0x76; data_w = 0x78; //78
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      // LNA load resistor - internal load (55)
      reg_addr = 0x79; data_w = 0x37;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      //####
      // Rx PLL + DSM
      //####
      // Output frequency
      reg_addr = 0x25; data_w = 0xd9;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      // CP current (1200uA)
      reg_addr = 0x26; data_w = 0x8c;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      // CP UP offset current (30uA)
      reg_addr = 0x27; data_w = 0xe3;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
      // N integer 
      reg_addr = 0x20; data_w = 0x7d;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);

      // N fractional over 3 registers
      reg_addr = 0x21; data_w = 0x00;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);

      reg_addr = 0x22; data_w = 0x00;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);

      reg_addr = 0x23; data_w = 0x00;
      write_lms6002_reg(reg_addr, data_w);
      data_r = read_lms6002_reg(reg_addr);
      uart_hd_putchar(data_r);
      
    }
    
    else if(ch=='r'){ // hardware reset
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
        write_lms6002_reg(0x29, i + 0x80);
        for(uint8_t delay=0; delay<100; delay++)__NOP();
        
        // read vtune
        curr_vtune = read_lms6002_reg(0x2a)>>6;
        
        // find cmin cmax
        if(last_vtune==0x2 && curr_vtune==0x0)
          cmin = i;
        else if(last_vtune==0x0 && curr_vtune==0x1){
          cmax = i;
          write_lms6002_reg(0x29, ((cmin+cmax)/2) + 0x80);
          break;
        }
        last_vtune = curr_vtune;
      }
      uart_hd_putchar(cmin);
      uart_hd_putchar(cmax);
    }
    /*
    else if(ch=='c'){ // calibrations
      //####
      // General DC Calibration
      //####
      uint16_t autocal_try_ctr = 0;
      uint8_t clbr_result = 0; // 0 'false', 1 'true'

      write_lms6002_reg(0x03, 0x08); // DC_ADDR = 0
      write_lms6002_reg(0x03, 0x28); // DC_START_CLBR = 1 
      write_lms6002_reg(0x03, 0x08); // DC_START_CLBR = 0

      // auto calibration loop
      while(1){
        for(uint8_t i=0; i<120; i++) // wait at least 6.4us
          __NOP(); 

        data_r = (read_lms6002_reg(0x01) >> 1) & 0x1; // read DC_CLBR_DONE
        
        // DC_CLBR_DONE branching
        if( data_r ) // DC_CLBR in progress
          autocal_try_ctr++;
        
        else if( !data_r){ // DC_CLBR done
          uint8_t dclock = (read_lms6002_reg(0x01) >> 2) & 0x7; //read DC_LOCK
          if( (dclock!=0) && (dclock!=7) ){
            clbr_result = 1;
            break;
          }
        }

        if(autocal_try_ctr>=350){
          clbr_result = 0;
          break;
        }
      }

      // output auto-calibration result
      uart_hd_putchar(clbr_result);

      //####
      // 
      //####

    }*/
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

