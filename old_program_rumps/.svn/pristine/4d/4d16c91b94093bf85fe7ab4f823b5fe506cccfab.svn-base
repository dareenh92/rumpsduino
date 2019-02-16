//------------------------------------------------------------------------------
//
// Main Program
// Application		: LCD
// Core						: Any
// Purpose
//	- LCD drive

#include "main.h"
void main_scheduler (void);
void draw_star_group(unsigned short color1, unsigned short color2, unsigned short turn);


unsigned char star_show [42] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
													 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
													};


unsigned char star_group [42] = { 0x01, 0x02, 0x00, 0x02, 0x03, 0x01, 
													 0x03, 0x01, 0x00, 0x02, 0x00, 0x02, 
													 0x01, 0x00, 0x03, 0x02, 0x03, 0x00, 
													 0x02, 0x03, 0x00, 0x03, 0x02, 0x01, 
													 0x03, 0x00, 0x02, 0x01, 0x03, 0x00, 
													 0x01, 0x02, 0x03, 0x02, 0x00, 0x03, 
													 0x02, 0x03, 0x01, 0x00, 0x02, 0x01
													};


volatile unsigned int  sleep_count = 0, sleep_time = 0;

//------------------------------------------------------------------------------
int main(void)
{
  	unsigned int i, j, k, speed, pressed;

	unsigned char string_a [26][5] = { "tree", "boat", "soap", "gear", "file", "keep",
														  "dull", "come", "four", "time", "mask", "quit",
														  "long", "once", "film", "rush", "bash", "fill",
														  "jump", "bias", "dark", "goat", "dirt", "most",
														  "pole", "weed"
														};
  
  	GPIO_OEN_CLR = 0x000080000;	// set Button input
  
  	MUXC_PU &= 0xfff7ffff;	// turn off PU for Button input

  	Lcd_Init();

	LCD_Clear(RED);

	j = 0;

	speed = 7;

	pressed = 0;

	sleep_time = 30;

	LCD_Show2Num(0xd0, 0x120, (7 - speed), 1, WHITE, RED);

	
	//-----------------------------------------------------
	GPIO_OEN_CLR = 0x1; 	// set GPIO_0 input
  
  	MUXC_PU &= 0xfffffffe;	// turn off PU for GPIO_0 output

	//Enable processor's interrupt
	
  	IC_NOC_CSR1 = 0x04000000; // turn on NOC RX interrupt
  
  	//set GPIO interrupt registers
  	GPIO_IT_CLR = 0x1; //level triggered
  	GPIO_IP_CLR = 0x1; //low-level
  	GPIO_IE_SET = 0x1; //GPIO_0

	GPIO_IF = 0x1;	// clear flag
	
	//* Ext INT 8 - GPIO0
	NVIC_SetPriority(8, 0);	
	NVIC_EnableIRQ(8);

	
	//--------------------------------------------------
	//Timer Settings
	TM_PR = 0x400; 			// prescale counter target value
	TM_COMR0 = 0x3D09; 	// timer counter target value, channel 0
	TM_COMCR = 0x3; 		// choose reset/stop upon reaching target value, enable interrupt
	
	TM_CTRL = 0x3; 	// use PCLK, reset timer, enable timer
	TM_CTRL = 0x1; 	// use PCLK, start timer, enable timer


	NVIC_SetPriority(6, 0);		// set Ext INT 6 (timer) priority  
	NVIC_EnableIRQ(6);			  // enable Ext INT 6 (timer) 
  

	//--------------------------------------------------	
	while (1) {
		//LCD_Clear(WHITE);
		//LCD_Clear(RED);
		/*LCD_Clear(YELLOW);
		LCD_Clear(GREEN);
		LCD_Clear(BLUE);
		LCD_Clear(RED);
		LCD_Clear(CYAN);
		LCD_Clear(BROWN);
		LCD_Clear(BLACK);*/	

		/*LCD_DrawPoint_big(0x60, 0x60, GREEN);
		LCD_DrawPoint_big(0x68, 0x68, BLUE);
		LCD_DrawPoint_big(0x70, 0x70, BLACK);

		LCD_DrawLine (0x20, 0x20, 0x50, 0x100, BLUE);
		LCD_DrawLine (0x40, 0x20, 0x20, 0x80, GREEN);

		LCD_DrawRectangle (0x44, 0x44, 0x54, 0x54, WHITE);

		LCD_DrawCircle (0x80, 0x80, 0x10, BLACK);

		for (j = 0; j < 26; j++) {

			LCD_ShowChar(0xa0, 0xa0, ('A' + j), 0x0, BLUE, RED);

			LCD_ShowChar(0xb0, 0xa0, ('z' - j), 0x0, BLUE, YELLOW);

			LCD_Show2Num(0xc0, 0xa0, (12+j), 4, WHITE, RED);

			LCD_ShowNum(0x60, 0xb0, (111111+j), 8, BLUE, RED);
	
			LCD_ShowString(0x10, 0xc0, string_a[j], GREEN, RED);

			for (k = 0; k < 10; k++){	
				LCD_DrawStar (0x60, 0xd0, (WHITE), RED);

				for (i = 0; i < 50000; i++){
					__NOP();
				}

				LCD_DrawStar (0x60, 0xd0, (GRAY), RED);

				for (i = 0; i < 50000; i++){
					__NOP();
				}
			}
		}*/
		for (k = 0; ; k++){	
			draw_star_group(WHITE, LGRAYBLUE, (k%4));

			for (i = 0; i < 5000; i++){
				__NOP();
			}

			if ( k % (4 + (speed * 3)) == 0) {
				if (star_show[j])
					star_show[j] = 0;
				else
					star_show[j] = 1;

				j = j +1;

				if (j == 42)
					j = 0;
			}

			//---------------------------------------------------------
			// button
			if (((GPIO_DATAIN & 0x00080000) == 0x00000000) && (pressed == 0x0)){
				pressed = 1;
			}
			else if (((GPIO_DATAIN & 0x00080000) == 0x00080000) && (pressed == 0x1)){
				pressed = 0;

				if (speed != 0)
					speed = speed - 1;
				else
					speed = 7;

				LCD_Show2Num(0xd0, 0x120, (7 - speed), 1, WHITE, RED);
			}

			//---------------------------------------------------------
			if (sleep_count == sleep_time)
			{
				__WFI();

				sleep_time += 30;
			}
		}
	}	
   
	return 0;
}

void draw_star_group(unsigned short color1, unsigned short color2, unsigned short turn)
{
	unsigned short i, j, k;

	for (i = 0; i < 7; i++){
		for (j = 0; j < 6; j++){
			k = (i*6) + j;

			if (star_show[k] == 1){
				if (star_group [k] == turn)
					LCD_DrawStar24 ((0x0c + (0x24 * j)), (0x0c + (0x24 * i)), color2, RED);
				else
					LCD_DrawStar24 ((0x0c + (0x24 * j)), (0x0c + (0x24 * i)), color1, RED);
			}
			else
				LCD_Fill((0x0c + (0x24 * j)), (0x0c + (0x24 * i)), (0x24 + (0x24 * j)), (0x24 + (0x24 * i)), RED);
		}
	}
		
}
  

