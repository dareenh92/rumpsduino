#ifndef __SPI_H_
#define __SPI_H_

#include <stdio.h>

#define SPI_DATA 	  ((*(volatile unsigned long *)	(0x40002000)))
#define SPI_STATE 	  ((*(volatile unsigned long *)	(0x40002004)))
#define SPI_CTRL 	  ((*(volatile unsigned long *)	(0x40002008)))
#define SPI_CRSEL 	  ((*(volatile unsigned long *)	(0x4000200c)))


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
		 	  unsigned char FSB, unsigned char SS, unsigned char CR);

void spi_enable();

void spi_disable();

void spi_send(unsigned int data);

unsigned int spi_recv();

#endif


