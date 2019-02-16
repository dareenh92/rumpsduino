//------------------------------------------------------------------------------
//
// Main Program
// Application		: lms6002d_txtest
// Core						: Any
// Purpose
//	- Configuring LMS6002D registers for TX test

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
    
    if(ch=='s'){
      // ### TX Chain ###
      // TOP Level
      write_lms6002_reg(0x05, 0x3a); // Soft tx/rx enable
      uart_hd_putchar(read_lms6002_reg(0x05));

      write_lms6002_reg(0x09, 0xc5); // Clock buffers - Tx/Rx DSM SPI
      uart_hd_putchar(read_lms6002_reg(0x09));
      
      // Tx LPF
      write_lms6002_reg(0x34, 0x3e); // select LPF bandwidth (.75MHz)
      uart_hd_putchar(read_lms6002_reg(0x34));
      
      // Tx RF
      write_lms6002_reg(0x41, 0x19); // VGA1 gain (-10dB)
      uart_hd_putchar(read_lms6002_reg(0x41));
      
      write_lms6002_reg(0x45, 0x78); // VGA2 gain (15dB)
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
      uart_hd_putchar(cmin);
      uart_hd_putchar(cmax);
      
      // ### RX Chain ###
      // Rx LPF
      write_lms6002_reg(0x54, 0x3e); // select LPF bandwidth (.75MHz)
      uart_hd_putchar(read_lms6002_reg(0x54));
      
      // Rx VGA2
      write_lms6002_reg(0x65, 0x01); // VGA2 gain (3dB)
      uart_hd_putchar(read_lms6002_reg(0x65));
      
      // Rx FE
      write_lms6002_reg(0x75, 0xd0); // active LNA=LNA1, LNA gain mode=max gain
      uart_hd_putchar(read_lms6002_reg(0x75));
      
      write_lms6002_reg(0x76, 0x78); // VGA1 control feedback resistor (120)
      uart_hd_putchar(read_lms6002_reg(0x76));
      
      write_lms6002_reg(0x79, 0x37); // LNA load resistor - internal load (55)
      uart_hd_putchar(read_lms6002_reg(0x79));
      
      // Rx PLL + DSM
      write_lms6002_reg(0x25, 0xd9); // Output frequency
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

      // auto vco cap tuning
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

    else if(ch=='r'){
      //hardware reset
      digitalWrite_low(RESETn);
      for (int i=0; i<1000; i++)__NOP();
      digitalWrite_high(RESETn);
    }

    else if(ch=='v'){
      write_lms6002_reg(0x19, 0x1a + 0x80);
      write_lms6002_reg(0x29, 0x27 + 0x80);
    }

    else if(ch=='d'){
      // toggle dac state
      uint8_t temp = read_lms6002_reg(0x57); // read register state
      uint8_t dac_state = (temp>>7) & 0x1;
      dac_state = ~dac_state;
      temp &= 0x7f; // clear 7th bit
      temp |= ((dac_state&0x1) << 7);

      write_lms6002_reg(0x57, temp);

      uart_hd_putchar(temp);
    }

    else if(ch=='c'){ // calibrations - in sequence

      uint8_t topspi_clken;
      uint8_t dccal, rccal;
      uint8_t clbr_result;

      //####
      // (1) DC offset calibration of LPF Tuning Module
      //####      
      
      topspi_clken = read_lms6002_reg(0x09); // save TOP::CLK_EN
      write_lms6002_reg(0x09, topspi_clken|0x20); // TOP::CLK_EN[5]=1

      //-----------
      // DC Calibration, TOP module
      clbr_result = 0; // 0 'false', 1 'true'

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

      // End of DC calibration, TOP module
      //-----------

      // check TOP DC calibration result
      if(clbr_result==0){ // PANIC: algo doesnt converge
        uart_hd_putchar(0x1f);
      }
      else if(clbr_result==1){ // SUCCESS: algo converge
        uart_hd_putchar(0x11);
        
        dccal = read_lms6002_reg(0x00); // DCCAL = TOP:DC_REGVAL
        write_lms6002_reg(0x55, dccal); // RXLPF::DCO_DACCAL = DCCAL
        write_lms6002_reg(0x35, dccal); // TXLPF::DCO_DACCAL = DCCAL
        write_lms6002_reg(0x09, topspi_clken); // restore TOP:CLK_EN

        uart_hd_putchar(dccal);
      }

      //####
      // (2) LPF Bandwidth Tuning procedure
      //####

      //------------
      // If PLL ref clock is not 40MHz, do these

      // power down txvga2 (opt)
      
      // enable TxPLL and set to produce 320MHz
     
      // use 40MHz from TxPLL, TOP::CLKSEL_LPFCAL = 0

      // power up LPF tuning clock genblock, TOP::PD_CLKLPFCAL = 0

      //-------------
      // Common procedures

      write_lms6002_reg(0x07, 0x0f); // TOP::BWC_LPFCAL, .75MHz
      write_lms6002_reg(0x07, 0x8f); // TOP::EN_CAL_LPFCAL = 1 (enable)

      write_lms6002_reg(0x06, 0x0d); // TOP::RST_CAL_LPFCAL = 1 (rst_active)
      __NOP(); __NOP();
      write_lms6002_reg(0x06, 0x0c); // TOP::RST_CAL_LPFCAL = 0 (rst_inactive)
      __NOP(); __NOP();

      rccal = read_lms6002_reg(0x01); // RCCAL = TOP::RCCAL_LPFCAL
      rccal = (rccal>>5)& 0x7;
      
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

      write_lms6002_reg(0x07, 0x0f); // TOP::EN_CAL_LPFCAL = 0 (disable)

      uart_hd_putchar(rccal);

      //####
      // (3) TXLPF DC Offset Calibration
      //####
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

      //####
      // (4) RXLPF DC Offset Calibration
      //####
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

      //####
      // (5) RXVGA2 DC Offset Calibration
      //####
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

    }
    
    else if(ch==']'){ // TX vga1 dc shift up, I channel
      data_r = read_lms6002_reg(0x42) + 0x1;
      write_lms6002_reg(0x42, data_r);
      uart_hd_putchar(data_r);
    }

    else if(ch=='['){ // TX vga1 dc shift down, I channel
      data_r = read_lms6002_reg(0x42) - 0x1;
      write_lms6002_reg(0x42, data_r);
      uart_hd_putchar(data_r);
    }

    else if(ch=='.'){ // TX vga1 dc shift up, Q channel
      data_r = read_lms6002_reg(0x43) + 0x1;
      write_lms6002_reg(0x43, data_r);
      uart_hd_putchar(data_r);
    }

    else if(ch==','){ // TX vga1 dc shift down, Q channel
      data_r = read_lms6002_reg(0x43) - 0x1;
      write_lms6002_reg(0x43, data_r);
      uart_hd_putchar(data_r);
    }

    else if(ch=='='){
      data_r = read_lms6002_reg(0x19) + 0x1;
      write_lms6002_reg(0x19, data_r);
      uart_hd_putchar(data_r - 0x80);
    }

    else if(ch=='-'){
      data_r = read_lms6002_reg(0x19) - 0x1;
      write_lms6002_reg(0x19, data_r);
      uart_hd_putchar(data_r - 0x80);
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

