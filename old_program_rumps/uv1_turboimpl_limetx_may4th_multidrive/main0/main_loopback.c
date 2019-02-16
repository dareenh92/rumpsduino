//------------------------------------------------------------------------------

//

// Main Program
// Application    : turboimpl_ioc
// Core           : IO Core
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > recv input seq, govern operations

// ### Interfacing with LMS6002D, TX part (with loopback) ###

#include "main.h"
#include "turbo_rumps_c0.h"

#define TURBO_MAX_ITR 1

#define IO_CHNLCTRL_HDR 0x1
#define IO_BITS_HDR 0x2
#define IO_LLRACK_HDR 0x3
#define IO_NOVAR_HDR 0x4
#define IO_STARTTURBO_HDR 0xa
#define IO_RDY 0xb
#define IO_TX 0xc

#define DSP_LLR_HDR 0x31
#define DSP_BITSACK_HDR 0x32

//shared with isr0.c
volatile uint8_t do_tx = 0; 
volatile uint8_t do_rx = 0;
volatile uint8_t rxd_ready = 0;
unsigned char codeword[768];
unsigned char send_pkt[776];
int16_t rxd_buff[776];

void main_scheduler (void);

// LMS6002d register access function
void write_lms6002_reg(uint8_t reg_addr, uint8_t data);
uint8_t read_lms6002_reg(uint8_t reg_addr);

//------------------------------------------------------------------------------

int main(void)

{
  // Set GPIOs & MUX
  MUXC_SELECT = 0x40; // SPI, GPIO[27:0]
  MUXC_PU &= 0x0f000000;  // SPI, GPIO[23:0] PU disabled

  //Timer Settings - Lime interfacing
  TM_PR = 0x1f3f; // prescale target, 1kHz (1f3f)
  TM_COMR0 = 0x0; //timer target, ch 0
  TM_COMCR = 0x3; // reset on COMR, enable interrupt

  TM_CTRL = 0x3; // use PCLK, reset timer, enable timer
  
  asm("movs  r0, #78\n"      // 1 cycle
      "loop: sub  r0, r0, #1\n" // 1 cycle
      "cmp r0, #0\n"         // 1 cycle
      "bne  loop\n");          // 2 if taken, 1 otherwise
  
  TM_CTRL = 0x1; // use PCLK, start timer, enable timer

  GPIO_OEN_SET = 0xfff; // output, GPIO[11:0]
  GPIO_OEN_CLR = 0xfff000; //input, GPIO[23:12]
  pinMode_input(26); // tweak - TX_IQSEL
  pinMode_input(27); // tweak - RX_IQSEL

  NVIC_SetPriority(6, 0);   // set Ext INT 6 (timer) priority

  // Initialization: Trellis, SPI, UART
  set_trellis();
  uart_hd_init_uart(1); // set pinMode 24 & 25
         // CPHA, CPOL, BC,   IE,   FSB,  SS,   CR
  spi_init( 0x0,  0x0,  0x1,  0x0,  0x0,  0x1,  0x1);
  spi_enable();

  // Define variables
  short recv_i = 0;   //keeps track of received channel bits 
  
  // Channel data's receive buffer
  unsigned char recv_sbit[256];  //received systematic bit
    
  unsigned char recv_done = 0; //flag of a data frame is fully received
  
  unsigned char I_last_send = intrlv_Im1; //latest permutated index for DRP interleaver
  unsigned char I_last_recv = intrlv_Im1; //latest permutated index for DRP interleaver

  // Fixed preamble format (1101_1011)
  /*send_pkt[0] = send_pkt[1] = send_pkt[3] = send_pkt[4] = 1;
  send_pkt[6] = send_pkt[7] = 1;
  send_pkt[2] = send_pkt[5] = 0;*/
  // 1101_1111
  send_pkt[0] = send_pkt[1] = send_pkt[2] = send_pkt[3] = 1;
  send_pkt[4] = send_pkt[6] = send_pkt[7] = 1;
  send_pkt[5] = 0;

  // TXD initial state
  GPIO_DATAOUT = (GPIO_DATAOUT & 0xfffff000);

  // *** Init Loop - Lime's initialization part ***
  while(1){
    char temp_uart;

    temp_uart = uart_hd_getchar(); //wait for command
    if(temp_uart=='s'){
      // TOP Level
      write_lms6002_reg(0x05, 0x3e); // Soft tx/rx enable
      uart_hd_putchar(read_lms6002_reg(0x05));

      // ### TX Chain ###
      write_lms6002_reg(0x09, 0xc5); // Clock buffers - Tx/Rx DSM SPI
      uart_hd_putchar(read_lms6002_reg(0x09));
      
      // Tx LPF
      write_lms6002_reg(0x34, 0x3e); // select LPF bandwidth (.75MHz)
      uart_hd_putchar(read_lms6002_reg(0x34));
      
      // Tx RF
      write_lms6002_reg(0x41, 0x15); // VGA1 gain (-14dB)
      uart_hd_putchar(read_lms6002_reg(0x41));
      
      write_lms6002_reg(0x45, 0x00); // VGA2 gain (0dB) bit 7-3
      uart_hd_putchar(read_lms6002_reg(0x45));
      
      write_lms6002_reg(0x44, 0x0b); // Select PA1
      uart_hd_putchar(read_lms6002_reg(0x44));
      
      // Tx PLL + DSM
      write_lms6002_reg(0x15, 0xfd); // Output frequency
      uart_hd_putchar(read_lms6002_reg(0x15));
      
      write_lms6002_reg(0x16, 0x8c); // CP current (1200uA)
      uart_hd_putchar(read_lms6002_reg(0x16));
      
      write_lms6002_reg(0x17, 0xe3); // CP UP offset current (30uA)
      uart_hd_putchar(read_lms6002_reg(0x17));
      
      write_lms6002_reg(0x10, 0x56); // N integer 
      uart_hd_putchar(read_lms6002_reg(0x10));

      write_lms6002_reg(0x11, 0x99); // N fractional over 3 registers
      uart_hd_putchar(read_lms6002_reg(0x11));

      write_lms6002_reg(0x12, 0x99);
      uart_hd_putchar(read_lms6002_reg(0x12));

      write_lms6002_reg(0x13, 0x99);
      uart_hd_putchar(read_lms6002_reg(0x13));

      //write_lms6002_reg(0x19, 0x22 + 0x80); // tuned vco cap value
      //uart_hd_putchar(read_lms6002_reg(0x19));
      
      // auto vco cap tuning
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
      uart_hd_putchar(read_lms6002_reg(0x19));

      // ### RX Chain ###
      // Rx LPF
      write_lms6002_reg(0x54, 0x3e); // select LPF bandwidth (.75MHz)
      uart_hd_putchar(read_lms6002_reg(0x54));
      
      // Rx VGA2
      write_lms6002_reg(0x65, 0x03); // VGA2 gain (9dB)
      uart_hd_putchar(read_lms6002_reg(0x65));
      
      // Rx FE
      write_lms6002_reg(0x75, 0xd0); // active LNA=LNA1, LNA gain mode=max gain
      uart_hd_putchar(read_lms6002_reg(0x75));
      
      write_lms6002_reg(0x76, 0x78); // VGA1 control feedback resistor (120)
      uart_hd_putchar(read_lms6002_reg(0x76));
      
      write_lms6002_reg(0x79, 0x37); // LNA load resistor - internal load (55)
      uart_hd_putchar(read_lms6002_reg(0x79));
      
      // Rx PLL + DSM
      write_lms6002_reg(0x25, 0xfd); // Output frequency
      uart_hd_putchar(read_lms6002_reg(0x25));
      
      write_lms6002_reg(0x26, 0x8c); // CP current (1200uA)
      uart_hd_putchar(read_lms6002_reg(0x26));
      
      write_lms6002_reg(0x27, 0xe3); // CP UP offset current (30uA)
      uart_hd_putchar(read_lms6002_reg(0x27));
      
      write_lms6002_reg(0x20, 0x56); // N integer 
      uart_hd_putchar(read_lms6002_reg(0x20));

      write_lms6002_reg(0x21, 0x99); // N fractional over 3 registers
      uart_hd_putchar(read_lms6002_reg(0x21));

      write_lms6002_reg(0x22, 0x99);
      uart_hd_putchar(read_lms6002_reg(0x22));

      write_lms6002_reg(0x23, 0x99);
      uart_hd_putchar(read_lms6002_reg(0x23));

      //write_lms6002_reg(0x29, 0x22 + 0x80); // tuned vco cap value
      //uart_hd_putchar(read_lms6002_reg(0x29));
      
      // auto vco cap tuning
      last_vtune = 0x2;
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
      uart_hd_putchar(read_lms6002_reg(0x29));
      
      uint8_t topspi_clken;
      uint8_t clbr_result;

      // ### Turn off DAC before calibrations ###
      uint8_t temp = (read_lms6002_reg(0x57)) & 0x7f; // clear 7th bit
      write_lms6002_reg(0x57, temp);
      uart_hd_putchar(temp);

      // ### Calibrations ###
      //****
      // (1) DC offset calibration of LPF Tuning Module
      uint8_t dccal = 0x17;
      write_lms6002_reg(0x55, dccal); // RXLPF::DCO_DACCAL = DCCAL
      write_lms6002_reg(0x35, dccal); // TXLPF::DCO_DACCAL = DCCAL
      uart_hd_putchar(dccal);

      //****
      // (2) LPF Bandwidth Tuning procedure
      uint8_t rccal = 0x06;
      uint8_t data_r;

      // RXLPF::RCCAL_LPF = RCCAL
      data_r = read_lms6002_reg(0x56);
      data_r &= 0x8f; // clear out prev RCCAL
      data_r |= (rccal << 4); // assign calibrated RCCAL 
      write_lms6002_reg(0x56, data_r);
      
      // TXLPF::RCCAL_LPF = RCCAL
      data_r = read_lms6002_reg(0x36);
      data_r &= 0x8f; // clear out prev RCCAL
      data_r |= (rccal << 4); // assign calibrated RCCAL 
      write_lms6002_reg(0x36, data_r);

      uart_hd_putchar(rccal);

      //****
      // (3) TXLPF DC Offset Calibration
      topspi_clken = read_lms6002_reg(0x09); // save TOP::CLK_EN
      write_lms6002_reg(0x09, topspi_clken|0x2); // TOP::CLK_EN[1] = 1

      //-----------
      // DC Calibration, TXLPF module, channel I (ADDR=0)
      clbr_result = 0; // 0 'false', 1 'true'

      write_lms6002_reg(0x33, 0x08); // DC_ADDR = 0
      write_lms6002_reg(0x33, 0x28); // DC_START_CLBR = 1 
      write_lms6002_reg(0x33, 0x08); // DC_START_CLBR = 0
      
      if(read_lms6002_reg(0x30)==31){ // read DC_REGVAL
        
        write_lms6002_reg(0x30, 0x0); // DC_REG_VAL = 0;
        write_lms6002_reg(0x33, 0x28); // DC_START_CLBR = 1 
        write_lms6002_reg(0x33, 0x08); // DC_START_CLBR = 0
        
        if(read_lms6002_reg(0x30)!=0) // read DC_REGVAL
          clbr_result = 1;
      }
      else
        clbr_result = 1;

      uart_hd_putchar(clbr_result);
      
      // End of DC calibration, TXLPF module, channel I
      //-----------

      if(clbr_result==1){
        //-----------
        // DC Calibration, TXLPF module, channel Q (ADDR=1)
        clbr_result = 0; // 0 'false', 1 'true'

        write_lms6002_reg(0x33, 0x09); // DC_ADDR = 1
        write_lms6002_reg(0x33, 0x29); // DC_START_CLBR = 1 
        write_lms6002_reg(0x33, 0x09); // DC_START_CLBR = 0
        
        if(read_lms6002_reg(0x30)==31){ // read DC_REGVAL
          
          write_lms6002_reg(0x30, 0x0); // DC_REG_VAL = 0;
          write_lms6002_reg(0x33, 0x29); // DC_START_CLBR = 1 
          write_lms6002_reg(0x33, 0x09); // DC_START_CLBR = 0
          
          if(read_lms6002_reg(0x30)!=0) // read DC_REGVAL
            clbr_result = 1;
        }
        else
          clbr_result = 1;

        uart_hd_putchar(clbr_result);

        // End of DC calibration, TXLPF module, channel Q
        //-----------
      }

      // check TXLPF DC calibration result
      if(clbr_result==0){ // PANIC: algo doesnt converge
        uart_hd_putchar(0x2f);
      }
      else if(clbr_result==1){ // SUCCESS: algo converge
        uart_hd_putchar(0x21);
        write_lms6002_reg(0x09, topspi_clken); // restore TOP:CLK_EN
      }

      //****
      // (4) RXLPF DC Offset Calibration
      topspi_clken = read_lms6002_reg(0x09); // save TOP::CLK_EN
      write_lms6002_reg(0x09, topspi_clken|0x8); // TOP::CLK_EN[3] = 1

      //-----------
      // DC Calibration, RXLPF module, channel I (ADDR=0)
      clbr_result = 0; // 0 'false', 1 'true'

      write_lms6002_reg(0x53, 0x08); // DC_ADDR = 0
      write_lms6002_reg(0x53, 0x28); // DC_START_CLBR = 1 
      write_lms6002_reg(0x53, 0x08); // DC_START_CLBR = 0
      
      if(read_lms6002_reg(0x50)==31){ // read DC_REGVAL
        
        write_lms6002_reg(0x50, 0x0); // DC_REG_VAL = 0;
        write_lms6002_reg(0x53, 0x28); // DC_START_CLBR = 1 
        write_lms6002_reg(0x53, 0x08); // DC_START_CLBR = 0
        
        if(read_lms6002_reg(0x50)!=0) // read DC_REGVAL
          clbr_result = 1;
      }
      else
        clbr_result = 1;

      uart_hd_putchar(clbr_result);
      
      // End of DC calibration, RXLPF module, channel I
      //-----------

      if(clbr_result==1){
        //-----------
        // DC Calibration, RTXLPF module, channel Q (ADDR=1)
        clbr_result = 0; // 0 'false', 1 'true'

        write_lms6002_reg(0x53, 0x09); // DC_ADDR = 1
        write_lms6002_reg(0x53, 0x29); // DC_START_CLBR = 1 
        write_lms6002_reg(0x53, 0x09); // DC_START_CLBR = 0
        
        if(read_lms6002_reg(0x50)==31){ // read DC_REGVAL
          
          write_lms6002_reg(0x50, 0x0); // DC_REG_VAL = 0;
          write_lms6002_reg(0x53, 0x29); // DC_START_CLBR = 1 
          write_lms6002_reg(0x53, 0x09); // DC_START_CLBR = 0
          
          if(read_lms6002_reg(0x50)!=0) // read DC_REGVAL
            clbr_result = 1;
        }
        else
          clbr_result = 1;

        uart_hd_putchar(clbr_result);

        // End of DC calibration, RXLPF module, channel Q
        //-----------
      }

      // check RXLPF DC calibration result
      if(clbr_result==0){ // PANIC: algo doesnt converge
        uart_hd_putchar(0x3f);
      }
      else if(clbr_result==1){ // SUCCESS: algo converge
        uart_hd_putchar(0x31);
        write_lms6002_reg(0x09, topspi_clken); // restore TOP:CLK_EN
      }

      //****
      // (5) RXVGA2 DC Offset Calibration
      topspi_clken = read_lms6002_reg(0x09); // save TOP::CLK_EN
      write_lms6002_reg(0x09, topspi_clken|0x10); // TOP::CLK_EN[4] = 1

      //-----------
      // DC Calibration, DC ref channel (ADDR=0)
      clbr_result = 0; // 0 'false', 1 'true'

      write_lms6002_reg(0x63, 0x08); // DC_ADDR = 0
      write_lms6002_reg(0x63, 0x28); // DC_START_CLBR = 1 
      write_lms6002_reg(0x63, 0x08); // DC_START_CLBR = 0
      
      if(read_lms6002_reg(0x60)==31){ // read DC_REGVAL
        
        write_lms6002_reg(0x60, 0x0); // DC_REG_VAL = 0;
        write_lms6002_reg(0x63, 0x28); // DC_START_CLBR = 1 
        write_lms6002_reg(0x63, 0x08); // DC_START_CLBR = 0
        
        if(read_lms6002_reg(0x60)!=0) // read DC_REGVAL
          clbr_result = 1;
      }
      else
        clbr_result = 1;

      uart_hd_putchar(clbr_result);

      if(clbr_result==1){
        //-----------
        // DC Calibration, VGA2A, I channel (ADDR=1)
        clbr_result = 0; // 0 'false', 1 'true'

        write_lms6002_reg(0x63, 0x09); // DC_ADDR = 1
        write_lms6002_reg(0x63, 0x29); // DC_START_CLBR = 1 
        write_lms6002_reg(0x63, 0x09); // DC_START_CLBR = 0
        
        if(read_lms6002_reg(0x60)==31){ // read DC_REGVAL
          
          write_lms6002_reg(0x60, 0x0); // DC_REG_VAL = 0;
          write_lms6002_reg(0x63, 0x29); // DC_START_CLBR = 1 
          write_lms6002_reg(0x63, 0x09); // DC_START_CLBR = 0
          
          if(read_lms6002_reg(0x60)!=0) // read DC_REGVAL
            clbr_result = 1;
        }
        else
          clbr_result = 1;

        uart_hd_putchar(clbr_result);

        if(clbr_result==1){
          //-----------
          // DC Calibration, VGA2A, Q channel (ADDR=2)
          clbr_result = 0; // 0 'false', 1 'true'

          write_lms6002_reg(0x63, 0x0a); // DC_ADDR = 2
          write_lms6002_reg(0x63, 0x2a); // DC_START_CLBR = 1 
          write_lms6002_reg(0x63, 0x0a); // DC_START_CLBR = 0
          
          if(read_lms6002_reg(0x60)==31){ // read DC_REGVAL
            
            write_lms6002_reg(0x60, 0x0); // DC_REG_VAL = 0;
            write_lms6002_reg(0x63, 0x2a); // DC_START_CLBR = 1 
            write_lms6002_reg(0x63, 0x0a); // DC_START_CLBR = 0
            
            if(read_lms6002_reg(0x60)!=0) // read DC_REGVAL
              clbr_result = 1;
          }
          else
            clbr_result = 1;

          uart_hd_putchar(clbr_result);

          if(clbr_result==1){
            //-----------
            // DC Calibration, VGA2B, I channel (ADDR=3)
            clbr_result = 0; // 0 'false', 1 'true'

            write_lms6002_reg(0x63, 0x0b); // DC_ADDR = 3
            write_lms6002_reg(0x63, 0x2b); // DC_START_CLBR = 1 
            write_lms6002_reg(0x63, 0x0b); // DC_START_CLBR = 0
            
            if(read_lms6002_reg(0x60)==31){ // read DC_REGVAL
              
              write_lms6002_reg(0x60, 0x0); // DC_REG_VAL = 0;
              write_lms6002_reg(0x63, 0x2b); // DC_START_CLBR = 1 
              write_lms6002_reg(0x63, 0x0b); // DC_START_CLBR = 0
              
              if(read_lms6002_reg(0x60)!=0) // read DC_REGVAL
                clbr_result = 1;
            }
            else
              clbr_result = 1;

            uart_hd_putchar(clbr_result);

            if(clbr_result==1){
              //-----------
              // DC Calibration, VGA2B, Q channel (ADDR=4)
              clbr_result = 0; // 0 'false', 1 'true'

              write_lms6002_reg(0x63, 0x0c); // DC_ADDR = 4
              write_lms6002_reg(0x63, 0x2c); // DC_START_CLBR = 1 
              write_lms6002_reg(0x63, 0x0c); // DC_START_CLBR = 0
              
              if(read_lms6002_reg(0x60)==31){ // read DC_REGVAL
                
                write_lms6002_reg(0x60, 0x0); // DC_REG_VAL = 0;
                write_lms6002_reg(0x63, 0x2c); // DC_START_CLBR = 1 
                write_lms6002_reg(0x63, 0x0c); // DC_START_CLBR = 0
                
                if(read_lms6002_reg(0x60)!=0) // read DC_REGVAL
                  clbr_result = 1;
              }
              else
                clbr_result = 1;

              uart_hd_putchar(clbr_result);
            }
          }
        }
      }
      // End of DC calibration, RXVGA2 module
      //-----------

      // check RXVGA2 DC calibration result
      if(clbr_result==0){ // PANIC: algo doesnt converge
        uart_hd_putchar(0x4f);
      }
      else if(clbr_result==1){ // SUCCESS: algo converge
        uart_hd_putchar(0x41);
        write_lms6002_reg(0x09, topspi_clken); // restore TOP:CLK_EN
      }

      // ### Lo Leakage Manual cancellation
      write_lms6002_reg(0x42, 0x86); // I channel
      write_lms6002_reg(0x43, 0x91); // Q channel

      // ### Turn on DAC after calibrations ###
      temp = (read_lms6002_reg(0x57)) | 0x80; // set 7th bit
      write_lms6002_reg(0x57, temp);
      uart_hd_putchar(temp);
      
      // ### ADC/DAC MISC_CTRL ###
      // TX_sync = 0, IQ
      // RX_sync = 1, IQ
      write_lms6002_reg(0x5a, 0xa0);
      
      // ### RF Loopback, mix to LNA1 ###
      write_lms6002_reg(0x08, 0x01);
      uart_hd_putchar(read_lms6002_reg(0x08));

      // ### Set Aux PA gain ###
      temp = ((read_lms6002_reg(0x4c)) & 0xf0) | 0x0;
      write_lms6002_reg(0x4c, temp);
      uart_hd_putchar(temp);
      
      // ### Signal other cores to enter decoding state ###
      while(noc_IC_txbuff_isfull==1)__NOP();
      IC_NOC_TX_BUFF1 = IC_NOC_TX_BUFF2 = IC_NOC_TX_BUFF3 = IO_STARTTURBO_HDR; 
      
      int tempack;
      while(noc_IC_rxbuff1_av!=1)__NOP(); tempack = IC_NOC_RX_BUFF1;
      while(noc_IC_rxbuff2_av!=1)__NOP(); tempack = IC_NOC_RX_BUFF2;
      while(noc_IC_rxbuff3_av!=1)__NOP(); tempack = IC_NOC_RX_BUFF3;

      break;
    }
  }
  // *** END - Init Loop ***
  
  // prepare codeword - payload
  for(uint32_t i=0; i<768; i++){
    if(i%4==0)
      codeword[i] = 0;
    else
      codeword[i] = 1;
  }

  // pack payload to preambled packet
  for(uint32_t i=8; i<776; i++){
    send_pkt[i] = codeword[i-8];
  }

  // *** Main Loop - Turbo coding part ***
  while(1){
    // process received data, inverting if needed
    if(rxd_ready==1){
      int16_t preamble[8];
      uint8_t do_inverse = 0;

      for(uint8_t i=0; i<8; i++)
        preamble[i] = rxd_buff[i];
      /*
      // detect non-inverted preamble
      if(preamble[0]>0 && preamble[1]>0 && preamble[2]<0 && preamble[3]>0 &&
         preamble[4]>0 && preamble[5]<0 && preamble[6]>0 && preamble[7]>0){
        do_inverse = 0;
      }
      //detect inverted preamble
      else if (preamble[0]>0 && preamble[1]>0 && preamble[2]<0 && preamble[3]>0 &&
               preamble[4]>0 && preamble[5]<0 && preamble[6]>0 && preamble[7]>0){
        do_inverse = 1;
      }

      if(do_inverse==1){
        for(uint32_t i=0; i<776; i++)
          rxd_buff[i] = ~rxd_buff[i] + 0x1;
      }
      */
      for(uint32_t i=0; i<776; i++)
        uart_hd_putchar((rxd_buff[i]>>4) & 0xff);

      rxd_ready = 0;
    }

    uart_hd_getchar(); // wait for command

    do_tx = 1; // start data transfer
    NVIC_EnableIRQ(6); // enable Ext INT 6 (timer)

    while(do_tx==1 || do_rx==1); // wait while tx in progress
  }
  // *** END - Main Loop ***

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
