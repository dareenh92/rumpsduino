//------------------------------------------------------------------------------
//
// Main Program
// Application    : seven segment
// Core            : Any
// Purpose
//  -

#include "main.h"

void main_scheduler (void);

volatile unsigned int  sleep_count = 0, sleep_time = 0;

volatile unsigned char pressed = 0, sleep = 0;

//------------------------------------------------------------------------------
int main(void)
{
  unsigned int i, j, k, speed;

  unsigned int speed_delay[8] = {3, 5, 8, 14, 24, 40, 65, 100};

  GPIO_OEN_CLR = 0x04;  // set Button input

  MUXC_PU &= 0xfb;  // turn off PU for Button input

  j = 0;

  speed = 7;

  pressed = 0;

  sleep_time = 21*16;

  SSEG_Init();

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
  NVIC_EnableIRQ(4);        // enable Ext INT 4 (timer)*/

  SSEG[0] = 0;
  SSEG[1] = 0;
  SSEG[2] = 0;
  SSEG[3] = 7 - speed;

  //--------------------------------------------------
  while (1) {
    for (k = 0; ; k++){

      for (i = 0; i < 50; i++){
        SSEG_Display ();
      }

      if ( (k % (speed_delay[speed] * 4)) == 0) {
        if (SSEG[0] != 9){
          SSEG[0] += 1;
        }
        else{
          if (SSEG[1] != 9){
            SSEG[0] = 0;
            SSEG[1] += 1;
          }
          else{
            if (SSEG[2] != 9){
              SSEG[0] = 0;
              SSEG[1] = 0;
              SSEG[2] += 1;
            }
            else{
              SSEG[0] = 0;
              SSEG[1] = 0;
              SSEG[2] = 0;
            }
          }
        }

        SSEG_Display ();
      }

      //---------------------------------------------------------
      // button
      if (pressed == 0x1){
        pressed = 0;

        if (speed != 0)
          speed = speed - 1;
        else
          speed = 7;

        SSEG[3] = 7 - speed;
      }

      //---------------------------------------------------------
      if (sleep_count == sleep_time)
      {
    	sleep = 1;

        __WFI();
      }
    }
  }

  return 0;
}
  
