//------------------------------------------------------------------------------
//
#include "main.h"


/*SPI access functions
 */


//---------------------------------------------------------
// CPHA : Clock Phase
// CPOL : Clock Polarity
// IE	: Interrupt Enable
// BC	: Byte Count ( 1-4)
// FSB  : First Significant Bit
// SS	: Control SS
// CR	: Clock Divider
//---------------------------------------------------------
void spi_init(unsigned char CPHA, unsigned char CPOL, unsigned char BC, unsigned char IE,
		 	  unsigned char FSB, unsigned char SS, unsigned char CR)
{
  unsigned int ctrl = 0x0;

  ctrl = SS & 0x1;
  ctrl <<= 0x1;

  ctrl |= FSB & 0x1;
  ctrl <<= 0x2;

  ctrl |= BC & 0x3;
  ctrl <<= 0x1;

  ctrl |= IE & 0x1;
  ctrl <<= 0x1;

  ctrl |= ((SPI_CTRL >> 2) & 0x1); // Enable
  ctrl <<= 0x1;

  ctrl |= CPOL & 0x1;
  ctrl <<= 0x1;

  ctrl |= CPHA & 0x1;

  SPI_CTRL = ctrl;
  SPI_CRSEL = CR;
}

void spi_enable()
{
  SPI_CTRL |= 0x4;
}

void spi_disable()
{
  SPI_CTRL &= 0xfffffffb;
}

void spi_send(unsigned int data)
{
  while ((SPI_STATE & 0x2) == 0x2) // wait SP_TIP low
    __NOP();

  SPI_DATA = data;

}
unsigned int spi_recv()
{
  while ((SPI_STATE & 0x2) == 0x2) // wait SP_TIP low
    __NOP();

  SPI_DATA = 0x0;

  while ((SPI_STATE & 0x1) == 0x0) // wait SP_IF high
    __NOP();

  SPI_STATE |= 0x1; // write 1 to clear

  return SPI_DATA;
}
