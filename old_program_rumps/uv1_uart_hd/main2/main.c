//------------------------------------------------------------------------------
//
// Main Program
// Application		: blinky
// Core						: Any
// Purpose
//	- blink an LED using corresponding core

#include "main.h"

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
	char ch, j;
  
  	GPIO_OEN_SET = 0x01;	// set as output

	//initialize uart
  	uart_hd_init_uart(0x0); // init GPIO and Timer

	for (j = 0; j < 3; j++){
		for (i = 0; i < 100000; i++)
			__NOP();

		GPIO_BTGL = 0x01;

		for (i = 0; i < 100000; i++)
			__NOP();

		GPIO_BTGL = 0x01;
	}
  
  	while(1)
  	{
		ch = uart_hd_getchar(0x0);

		for (j = 0; j < ch; j++){
			for (i = 0; i < 100000; i++)
	  			__NOP();

			GPIO_BTGL = 0x01;

			for (i = 0; i < 100000; i++)
	  			__NOP();

			GPIO_BTGL = 0x01;
		}
		
			uart_hd_putchar('0');
			uart_hd_putchar('1');
			uart_hd_putchar('2');
			uart_hd_putchar('3');
			uart_hd_putchar('4');
			uart_hd_putchar('5');
			uart_hd_putchar('6');
			uart_hd_putchar('7');
		
  	}
   
  	return 0;
}
  
