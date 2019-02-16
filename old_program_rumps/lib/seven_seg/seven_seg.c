//------------------------------------------------------------------------------
//
#include "main.h"

#include "type.h"
#include "seven_seg.h"

//---------------------------------------------
/*

SSEG_DIO    = GPIO1_6
SSEG_RCLK   = GPIO1_5
SSEG_SCLK   = GPIO1_4
*/

						  // 0     1     2     3     4     5     6     7     8     9
u8 fseg[10] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};
u8 segbit[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
u8 disbuf[4]={0, 0, 0, 0};

u8 SSEG_0F[17] =
 {// 0     1     2     3     4     5     6     7     8     9
     0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90,
  //  A     b    C     d     E     F     -
     0x8C, 0xBF, 0xC6, 0xA1, 0x86, 0xFF, 0xbf
 };


//-----------------------------------------------------------------------------
//


//*****************************************************************************
//
//
void SSEG_Init(void)
{
  //GPIO_OEN_SET = 0x70;	// set SSEG control output
  GPIO_OEN_SET = SSEG_DIO | SSEG_RCLK | SSEG_SCLK;	// set SSEG control output

  //MUXC_PU &= 0x8f;	// turn off PU for SSEG control output
  MUXC_PU &= ~(SSEG_DIO | SSEG_RCLK | SSEG_SCLK);	// turn off PU for SSEG control output

  GPIO_BCLR = SSEG_DIO;
  GPIO_BCLR = SSEG_RCLK;
  GPIO_BCLR = SSEG_SCLK;

}


//*****************************************************************************
//
//
void SSEG_Display (void)
{
  u8 *sseg_table;          //
  u8 i;

  sseg_table = SSEG_0F + SSEG[0];

  i = *sseg_table;

  SSEG_OUT(i);
  SSEG_OUT(0x01);

  GPIO_BCLR = SSEG_RCLK;
  GPIO_BSET = SSEG_RCLK;

  sseg_table = SSEG_0F + SSEG[1];
  i = *sseg_table;

  SSEG_OUT(i);
  SSEG_OUT(0x02);

  GPIO_BCLR = SSEG_RCLK;
  GPIO_BSET = SSEG_RCLK;

  sseg_table = SSEG_0F + SSEG[2];
  i = *sseg_table;

  SSEG_OUT(i);
  SSEG_OUT(0x04);

  GPIO_BCLR = SSEG_RCLK;
  GPIO_BSET = SSEG_RCLK;

  sseg_table = SSEG_0F + SSEG[3];
  i = *sseg_table;

  SSEG_OUT(i);
  SSEG_OUT(0x08);

  GPIO_BCLR = SSEG_RCLK;
  GPIO_BSET = SSEG_RCLK;
}

//*****************************************************************************
//
//
void SSEG_OUT(u8 X)
{
  u8 i;

  for(i=8; i>=1; i--)
  {
    if (X&0x80)
    	GPIO_BSET = SSEG_DIO;
    else
    	GPIO_BCLR = SSEG_DIO;

    X<<=1;

    GPIO_BCLR = SSEG_SCLK;
    GPIO_BSET = SSEG_SCLK;
  }
}

