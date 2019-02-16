//------------------------------------------------------------------------------
//
// Main Program
// Application    : blinky + button
// Core            : Any
// Purpose
//  - blink an LED using corresponding core

#include "main.h"

void main_scheduler (void);

#define LED0    0x02
#define LED1    0x04
#define LED2    0x08

#define LED_RED    0x10
#define LED_GREEN  0x20
#define LED_BLUE   0x40

volatile unsigned int  sleep_count = 0, sleep_time = 0;

volatile unsigned char pressed = 0, sleep = 0, pressed_count = 0, long_pressed = 0;

volatile unsigned char noc_update = 0;

volatile unsigned int speed = 7;


//------------------------------------------------------------------------------
int main(void)
{
  unsigned int i, j, k, change_count;

  unsigned int speed_delay[8] = {3, 5, 8, 14, 24, 40, 65, 100};

  unsigned char d_cycle[3][32] =  {{0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0},
	  	  	  	  	  	  	  	   {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0},
	  	  	  	  	  	  	  	   {0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0,  0, 0, 0, 0}};

  unsigned char RGB_led[3] 	= {LED_RED, LED_GREEN, LED_BLUE};

  	  	  	  	  	  	  	  	  	// R     G     B
 /* unsigned char color_code[80][3] = {{0x16, 0x00, 0x00}, // RED
  			 	 	 	 	 	 	 {0x16, 0x01, 0x00}, //
  			 	 	 	 	 	 	 {0x15, 0x01, 0x00}, //
  			 	 	 	 	 	 	 {0x15, 0x02, 0x00}, //

  			 	 	 	 	 	 	 {0x14, 0x02, 0x00}, //
  			 	 	 	 	 	 	 {0x13, 0x02, 0x00}, //
  			 	 	 	 	 	 	 {0x12, 0x02, 0x00}, // ORANGE
    								 {0x12, 0x03, 0x00}, //

    								 {0x12, 0x04, 0x00}, //
    								 {0x12, 0x05, 0x00}, //
  			 	 	 	 	 	 	 {0x12, 0x06, 0x00}, //
  			 	 	 	 	 	 	 {0x11, 0x06, 0x00}, //

  			 	 	 	 	 	 	 {0x11, 0x07, 0x00}, // YELLOW
  			 	 	 	 	 	 	 {0x10, 0x07, 0x00}, //
    								 {0x10, 0x08, 0x00}, //
    								 {0x0a, 0x08, 0x00}, //

    								 {0x0a, 0x09, 0x00}, //
    								 {0x09, 0x09, 0x00}, //
    								 {0x09, 0x0a, 0x00}, //
    								 {0x08, 0x0a, 0x00}, // LIME

    								 {0x06, 0x0a, 0x00}, //
    								 {0x04, 0x0a, 0x00}, //
    								 {0x02, 0x0a, 0x00}, //
    								 {0x02, 0x0c, 0x00}, //

    								 {0x02, 0x0f, 0x00}, //
    								 {0x02, 0x12, 0x00}, // GREEN
    								 {0x00, 0x12, 0x01}, //
  			 	 	 	 	 	 	 {0x00, 0x10, 0x01}, //

    								 {0x00, 0x0e, 0x02}, //
    								 {0x00, 0x0c, 0x02}, //
    								 {0x00, 0x0b, 0x02}, //
    								 {0x00, 0x0a, 0x02}, //

    								 {0x00, 0x0a, 0x03}, //
    								 {0x00, 0x0a, 0x04}, // TURQUOISE
    								 {0x00, 0x0a, 0x05}, //
    								 {0x00, 0x0a, 0x06}, //

    								 {0x00, 0x09, 0x06}, //
    								 {0x00, 0x09, 0x07}, //
    								 {0x00, 0x08, 0x07}, //
    								 {0x00, 0x08, 0x09}, //

    								 {0x00, 0x07, 0x09}, //
    								 {0x00, 0x07, 0x0b}, // LIGHT BLUE
    								 {0x00, 0x06, 0x0b}, //
    								 {0x00, 0x06, 0x0d}, //

    								 {0x00, 0x05, 0x0d}, //
    								 {0x00, 0x05, 0x0f}, //
    								 {0x00, 0x04, 0x11}, //
    								 {0x00, 0x03, 0x13}, //

    								 {0x00, 0x02, 0x13}, //
    								 {0x00, 0x02, 0x14}, // BLUE
    								 {0x00, 0x02, 0x15}, //
    								 {0x00, 0x01, 0x15}, //

    								 {0x00, 0x00, 0x15}, //
    								 {0x01, 0x00, 0x15}, //
    								 {0x01, 0x00, 0x15}, //
    								 {0x02, 0x00, 0x13}, //

    								 {0x02, 0x00, 0x11}, //
    								 {0x02, 0x00, 0x0f}, //
    								 {0x02, 0x00, 0x0d}, // PURPLE
    								 {0x03, 0x00, 0x0d}, //

    								 {0x04, 0x00, 0x0d}, //
    								 {0x05, 0x00, 0x0d}, //
    								 {0x06, 0x00, 0x0d}, //
    								 {0x06, 0x00, 0x0b}, //

    								 {0x07, 0x00, 0x0b}, //
    								 {0x07, 0x00, 0x0a}, //
    								 {0x08, 0x00, 0x0a}, // ROSE
    								 {0x08, 0x00, 0x09}, //

    								 {0x09, 0x00, 0x09}, //
    								 {0x09, 0x00, 0x08}, //
    								 {0x0a, 0x00, 0x08}, //
    								 {0x0a, 0x00, 0x07}, //

    								 {0x10, 0x00, 0x06}, //
    								 {0x10, 0x00, 0x05}, //
    								 {0x12, 0x00, 0x04}, // PINK
    								 {0x12, 0x00, 0x03}, //

    								 {0x13, 0x00, 0x02}, //
    								 {0x13, 0x00, 0x01}, //
    								 {0x14, 0x00, 0x01}, //
    								 {0x15, 0x00, 0x01}, //

      	  	  	  	  	  	  	  	};*/

  unsigned char color_code[80][3] = {{0x1f, 0x00, 0x00}, // RED
    			 	 	 	 	 	 {0x1e, 0x00, 0x00}, //
    			 	 	 	 	 	 {0x1d, 0x00, 0x00}, //
    			 	 	 	 	 	 {0x1d, 0x01, 0x00}, //

    			 	 	 	 	 	 {0x1b, 0x01, 0x00}, //
    			 	 	 	 	 	 {0x1a, 0x01, 0x00}, //
    			 	 	 	 	 	 {0x19, 0x01, 0x00}, // ORANGE
      								 {0x19, 0x02, 0x00}, //

      								 {0x19, 0x02, 0x00}, //
      								 {0x19, 0x04, 0x00}, //
    			 	 	 	 	 	 {0x19, 0x06, 0x00}, //
    			 	 	 	 	 	 {0x17, 0x06, 0x00}, //

    			 	 	 	 	 	 {0x17, 0x07, 0x00}, // YELLOW
    			 	 	 	 	 	 {0x15, 0x07, 0x00}, //
      								 {0x15, 0x08, 0x00}, //
      								 {0x12, 0x08, 0x00}, //

      								 {0x12, 0x09, 0x00}, //
      								 {0x0f, 0x09, 0x00}, //
      								 {0x0f, 0x0a, 0x00}, //
      								 {0x0c, 0x0a, 0x00}, // LIME

      								 {0x0a, 0x0a, 0x00}, //
      								 {0x08, 0x0a, 0x00}, //
      								 {0x03, 0x0a, 0x00}, //
      								 {0x03, 0x0b, 0x00}, //

      								 {0x03, 0x0d, 0x00}, //
      								 {0x03, 0x10, 0x00}, // GREEN
      								 {0x00, 0x10, 0x02}, //
    			 	 	 	 	 	 {0x00, 0x0f, 0x02}, //

      								 {0x00, 0x0d, 0x03}, //
      								 {0x00, 0x0c, 0x03}, //
      								 {0x00, 0x0b, 0x03}, //
      								 {0x00, 0x0a, 0x03}, //

      								 {0x00, 0x0a, 0x05}, //
      								 {0x00, 0x0a, 0x06}, // TURQUOISE
      								 {0x00, 0x0a, 0x07}, //
      								 {0x00, 0x0a, 0x09}, //

      								 {0x00, 0x09, 0x09}, //
      								 {0x00, 0x09, 0x0a}, //
      								 {0x00, 0x08, 0x0a}, //
      								 {0x00, 0x08, 0x0c}, //

      								 {0x00, 0x07, 0x0c}, //
      								 {0x00, 0x07, 0x0f}, // LIGHT BLUE
      								 {0x00, 0x06, 0x0f}, //
      								 {0x00, 0x06, 0x11}, //

      								 {0x00, 0x05, 0x13}, //
      								 {0x00, 0x05, 0x15}, //
      								 {0x00, 0x04, 0x17}, //
      								 {0x00, 0x03, 0x19}, //

      								 {0x00, 0x02, 0x1b}, //
      								 {0x00, 0x02, 0x1d}, // BLUE
      								 {0x00, 0x02, 0x1f}, //
      								 {0x00, 0x01, 0x1f}, //

      								 {0x00, 0x00, 0x1f}, //
      								 {0x02, 0x00, 0x1f}, //
      								 {0x02, 0x00, 0x1d}, //
      								 {0x03, 0x00, 0x1d}, //

      								 {0x03, 0x00, 0x1b}, //
      								 {0x03, 0x00, 0x19}, //
      								 {0x03, 0x00, 0x17}, // PURPLE
      								 {0x04, 0x00, 0x17}, //

      								 {0x06, 0x00, 0x15}, //
      								 {0x08, 0x00, 0x15}, //
      								 {0x0a, 0x00, 0x15}, //
      								 {0x0a, 0x00, 0x13}, //

      								 {0x0c, 0x00, 0x13}, //
      								 {0x0c, 0x00, 0x11}, //
      								 {0x0e, 0x00, 0x11}, // ROSE
      								 {0x0e, 0x00, 0x0f}, //

      								 {0x11, 0x00, 0x0f}, //
      								 {0x11, 0x00, 0x0d}, //
      								 {0x13, 0x00, 0x0b}, //
      								 {0x13, 0x00, 0x09}, //

      								 {0x15, 0x00, 0x07}, //
      								 {0x15, 0x00, 0x06}, //
      								 {0x17, 0x00, 0x05}, // PINK
      								 {0x17, 0x00, 0x04}, //

      								 {0x19, 0x00, 0x03}, //
      								 {0x19, 0x00, 0x02}, //
      								 {0x1b, 0x00, 0x02}, //
      								 {0x1d, 0x00, 0x02}, //

        	  	  	  	  	  	  	 };

  unsigned int active_OEN = 0x0;
  unsigned int active_PU = 0x0;

  j = 0;

  speed = 7;

  pressed = 0;

  change_count = 0;

  sleep_time = 21*16;

  GPIO_OEN_SET = 0x7e;  // set LEDs output

  MUXC_PU &= 0x81;  // turn off PU for LEDs output

  GPIO_BSET = LED0;
  GPIO_BSET = LED1;
  GPIO_BSET = LED2;


  //-----------------------------------------------------
  GPIO_OEN_CLR = 0x1;   // set GPIO_0 input

  MUXC_PU &= 0xfe;  // turn off PU for GPIO_0 output

  //Enable processor's interrupt

  //set GPIO interrupt registers
  //GPIO_IT_SET = 0x1; // edge triggered
  GPIO_IT_CLR = 0x1; // level triggered
  GPIO_IP_CLR = 0x1; // low
  GPIO_IE_SET = 0x1; // GPIO_0

  GPIO_IF = 0x1;  // clear flag

  //* Ext INT 5 - GPIO0
  NVIC_SetPriority(5, 0);
  NVIC_EnableIRQ(5);


  //--------------------------------------------------
  //Timer Settings
  TM_PR = 0x400;       // prescale counter target value
  TM_COMR0 = 0x3D09/16;   // timer counter target value, channel 0
  TM_COMCR = 0x3;     // choose reset/stop upon reaching target value, enable interrupt

  TM_CTRL = 0x3;   // use PCLK, reset timer, enable timer
  TM_CTRL = 0x1;   // use PCLK, start timer, enable timer

  NVIC_SetPriority(3, 2);    // set Ext INT 3 (timer) priority
  NVIC_EnableIRQ(3);        // enable Ext INT 3 (timer)


  //--------------------------------------------------
  // NOC Settings
  NC_NOC_CSR1 = 0x04000000; // set NOC RX interrupt

  //* Ext INT 2 - NoC RX
  NVIC_SetPriority(2, 1);
  NVIC_EnableIRQ(2);


  //--------------------------------------------------
  while (1) {
    for (k = 0; ; k++){

      for (i = 0; i < 10; i++){
        __NOP();
      }

      /*if((TM_IF & 0x1) == 0x1){
        TM_IF = 0x0;

        sleep_count += 1;
      }*/

      if ( (k % (speed_delay[speed] * 160)) == 0) {
    	change_count += 1;

    	if (change_count == 80)
    		change_count = 0;

    	for (j = 0; j < color_code[change_count][0] + 0; j++){
          d_cycle[0][j] = 1;
    	}
    	for ( ; j < 32 ; j++){
    	  d_cycle[0][j] = 0;
    	}

    	for (j = 0; j < color_code[change_count][1] + 0; j++){
    	  d_cycle[1][j] = 1;
    	}
    	for ( ; j < 32 ; j++){
    	  d_cycle[1][j] = 0;
    	}

    	for (j = 0; j < color_code[change_count][2] + 0; j++){
    	  d_cycle[2][j] = 1;
    	}
    	for ( ; j < 32 ; j++){
    	  d_cycle[2][j] = 0;
    	}
      }


      //---------------------------------------------------------
      // button
      if (pressed == 0x1){
        pressed = 0;

        if (speed != 0)
          speed = speed - 1;
        else
          speed = 7;

        if ((speed & 0x1) != 0)
          GPIO_BSET = LED0;
        else
          GPIO_BCLR = LED0;

        if ((speed & 0x2) != 0)
          GPIO_BSET = LED1;
        else
          GPIO_BCLR = LED1;

        if ((speed & 0x4) != 0)
          GPIO_BSET = LED2;
        else
          GPIO_BCLR = LED2;
      }

      //---------------------------------------------------------
      if (noc_update == 0x1)
      {
    	noc_update = 0;

    	if ((speed & 0x1) != 0)
    	  GPIO_BSET = LED0;
    	else
    	  GPIO_BCLR = LED0;

    	if ((speed & 0x2) != 0)
    	  GPIO_BSET = LED1;
    	else
    	  GPIO_BCLR = LED1;

    	if ((speed & 0x4) != 0)
    	  GPIO_BSET = LED2;
    	else
    	  GPIO_BCLR = LED2;
      }

      //---------------------------------------------------------
      if (sleep_count == sleep_time)
      {
    	sleep = 1;
    	//GPIO_BSET = RGB_led[0];
    	//GPIO_BSET = RGB_led[1];
    	//GPIO_BSET = RGB_led[2];

    	GPIO_BCLR = RGB_led[0];
    	GPIO_BCLR = RGB_led[1];
    	GPIO_BCLR = RGB_led[2];

    	GPIO_BSET = LED0;
    	GPIO_BSET = LED1;
    	GPIO_BSET = LED2;

    	//active_OEN = GPIO_OEN_SET;
    	//active_PU = MUXC_PU;

    	//GPIO_OEN_CLR = 0xff;
    	//MUXC_PU = 0xfe;

        __WFI();

    	//MUXC_PU = active_PU;
    	//GPIO_OEN_SET = active_OEN;

        if ((speed & 0x1) != 0)
          GPIO_BSET = LED0;
        else
          GPIO_BCLR = LED0;

        if ((speed & 0x2) != 0)
          GPIO_BSET = LED1;
        else
          GPIO_BCLR = LED1;

        if ((speed & 0x4) != 0)
          GPIO_BSET = LED2;
        else
          GPIO_BCLR = LED2;
      }

      for (i = 0; i < 3; i++){
    	if (d_cycle[i][k%32]){
    	  //GPIO_BCLR = RGB_led[i];
    	  GPIO_BSET = RGB_led[i];
    	}
    	else{
      	  //GPIO_BSET = RGB_led[i];
    	  GPIO_BCLR = RGB_led[i];
    	}
      }
    }
  }

  return 0;
}
