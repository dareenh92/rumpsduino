//------------------------------------------------------------------------------
//
// Main Program
// Application    : blinky + button
// Core            : Any
// Purpose
//  - blink an LED using corresponding core

#include "main.h"

void main_scheduler (void);

u8 rom_num[36][8]={
	  {0x00,0x00,0x24,0x34,0x2c,0x24,0x00,0x00},//0
	  {0x00,0x00,0x08,0x08,0x08,0x08,0x00,0x00},//1
	  {0x00,0x00,0x14,0x14,0x14,0x14,0x00,0x00},//2
	  {0x00,0x00,0x2a,0x2a,0x2a,0x2a,0x00,0x00},//3
	  {0x00,0x00,0x2a,0x2a,0x2a,0x24,0x00,0x00},//4
	  {0x00,0x00,0x14,0x14,0x14,0x08,0x00,0x00},//5
	  {0x00,0x00,0x2a,0x2a,0x2a,0x12,0x00,0x00},//6
	  {0x00,0x00,0x55,0x55,0x55,0x25,0x00,0x00},//7
  	};

volatile unsigned int  sleep_count = 0, sleep_time = 0;

volatile unsigned char pressed = 0, sleep = 0;

  
//------------------------------------------------------------------------------
int main(void)
{
  unsigned int i, j, k, speed;

  unsigned int speed_delay[8] = {3, 5, 8, 14, 24, 40, 65, 100};

  unsigned char carry, carry2;

  GPIO_OEN_CLR = 0x04;  // set Button input

  MUXC_PU &= 0xfb;  // turn off PU for Button input

  j = 0;

  speed = 7;

  pressed = 0;

  sleep_time = 18*16;

  Init_MX();

  rom_num[7-speed][0] = 0x02;
  rom_num[7-speed][1] = 0xf0;

  rom_num[7-speed][6] = 0x70;
  rom_num[7-speed][7] = 0x0c;

  //-----------------------------------------------------
  GPIO_OEN_CLR = 0x1;   // set GPIO_0 input

  MUXC_PU &= 0xfe;  // turn off PU for GPIO_0 output

  //Enable processor's interrupt

  //set GPIO interrupt registers
  GPIO_IT_SET = 0x1; // edge triggered
  GPIO_IP_CLR = 0x1; // fall
  GPIO_IE_SET = 0x1; // GPIO_0

  GPIO_IF = 0x1;  // clear flag

  //* Ext INT 6 - GPIO0
  NVIC_SetPriority(6, 0);
  NVIC_EnableIRQ(6);

  //--------------------------------------------------
  //Timer Settings
  TM_PR = 0x400;       // prescale counter target value
  TM_COMR0 = 0x3D09/16;   // timer counter target value, channel 0
  TM_COMCR = 0x3;     // choose reset/stop upon reaching target value, enable interrupt

  TM_CTRL = 0x3;   // use PCLK, reset timer, enable timer
  TM_CTRL = 0x1;   // use PCLK, start timer, enable timer

  NVIC_SetPriority(4, 0);    // set Ext INT 4 (timer) priority
  NVIC_EnableIRQ(4);        // enable Ext INT 4 (timer)

  //--------------------------------------------------
  while (1) {
    for (k = 0; ; k++){

      for (i = 0; i < 200; i++){
        __NOP();
      }

      if ( (k % (speed_delay[speed] * 24)) == 0) {

    	//------
    	carry = rom_num[7-speed][0] & 0x80;

		rom_num[7-speed][0] <<= 1;

    	//------
    	carry2 = rom_num[7-speed][1] & 0x01;

    	rom_num[7-speed][1] >>= 1;

    	if (carry != 0)
    		rom_num[7-speed][1] |= 0x80;

    	if (carry2 != 0)
    		rom_num[7-speed][0] |= 0x01;

    	//------
    	carry = rom_num[7-speed][6] & 0x80;

    	rom_num[7-speed][6] <<= 1;

    	//------
    	carry2 = rom_num[7-speed][7] & 0x01;

    	rom_num[7-speed][7] >>= 1;

    	if (carry != 0)
    		rom_num[7-speed][7] |= 0x80;

    	if (carry2 != 0)
    		rom_num[7-speed][6] |= 0x01;
      }

      //---------------------------------------------------------
      // button
      if (pressed == 0x1){
        pressed = 0;

        if (speed != 0)
          speed = speed - 1;
        else
          speed = 7;

        rom_num[7-speed][0] = rom_num[(14-speed)%8][0];
        rom_num[7-speed][1] = rom_num[(14-speed)%8][1];

        rom_num[7-speed][6] = rom_num[(14-speed)%8][6];
        rom_num[7-speed][7] = rom_num[(14-speed)%8][7];
      }

      //---------------------------------------------------------
      if (sleep_count == sleep_time)
      {
    	sleep = 1;

        __WFI();
      }

      for(i=0; i<8; i++){
       	Write_MX(i+1,(rom_num[(7-speed)][i]));
      }
    }
  }

  return 0;
}
