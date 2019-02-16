//------------------------------------------------------------------------------
//
// Main Program
// Application		: blinky
// Core						: Any
// Purpose
//	- blink an LED using corresponding core

#include "main.h"
void main_scheduler (void);

#define ADC_RST		0x01000000
#define ADC_DRDY	0x02000000

#define ADC_DOUT	0x10000000
#define ADC_CS		0x20000000
#define ADC_SCK		0x40000000
#define ADC_DIN		0x80000000

unsigned char store[2];

//------------------------------------------------------------------------------
void AD7705_Write_reg(unsigned char byteword);

void AD7705_Read(unsigned char *datapointer, int reglength);


//------------------------------------------------------------------------------
int main(void)
{
  int i = 0;
  
  	  	 // CPHA, CPOL, BC,   IE,   FSB,  SS,   CR
  spi_init( 0x1,  0x1,  0x0,  0x0,  0x0,  0x1,  0x1);

  spi_enable();

  //SPI_CTRL = 0xc4;
  //SPI_CRSEL = 0x00;

  // mux
  MUXC_SELECT = 0x00000040;	// SPI selected
  MUXC_PU &= 0x0fffffff;	// SPI PU disabled

  GPIO_OEN_SET = ADC_RST;

  GPIO_OEN_CLR = ADC_DRDY;

  //initialize uart
  uart_hd_init_uart(); // init GPIO

  GPIO_BSET = ADC_RST;
  __NOP();
  __NOP();
  __NOP();
  GPIO_BCLR = ADC_RST;
  __NOP();
  __NOP();
  __NOP();
  GPIO_BSET = ADC_RST;



  AD7705_Write_reg(0x20); /* Active Channel is Ain1(+)/Ain1(-),next operation as write to the clock register */
  AD7705_Write_reg(0x0C); /* master clock enabled,4.9512MHz  Clock, set output  rate to 50Hz*/

  AD7705_Write_reg(0x10); /* Active Channel is Ain1(+)/Ain1(-),next operation as write to the setup register */
  //AD7705_Write_reg(0x40); /* gain = 1, bipolar mode, buffer off, clear FSYNC and perform a Self Calibration */
  AD7705_Write_reg(0x44); /* gain = 1, unipolar mode, buffer off, clear FSYNC and perform a Self Calibration */

  while((GPIO_DATAIN & ADC_DRDY) == ADC_DRDY)       /* wait for /DRDY  to go low */
    __NOP();

  while (1)
  {
	AD7705_Write_reg(0x38); /*set the next operation for 16 bit readfrom the data	register*/
	AD7705_Read(store, 2);

  	uart_hd_putchar(store[0], 0x1);
  	uart_hd_putchar(store[1], 0x1);

  	for (i = 0; i < 1000000; i++)
  	  __NOP();
  }
   
  return 0;
}


//--------------------------------------------
void AD7705_Write_reg(unsigned char byteword)
{
  spi_send(byteword);    /*    put the byte into data register*/
}


//--------------------------------------------
void AD7705_Read(unsigned char *datapointer, int reglength)
{
  int b;

  while((GPIO_DATAIN & ADC_DRDY) == ADC_DRDY)       /* wait for /DRDY  to go low */
    __NOP();

  for(b = 0; b < reglength; b++)
  {
    *datapointer++ = spi_recv(); /* read SPDR into store array via datapointer */
  }
}


  

