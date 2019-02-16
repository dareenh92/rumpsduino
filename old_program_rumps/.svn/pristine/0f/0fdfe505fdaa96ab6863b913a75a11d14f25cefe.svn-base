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
  
  	//initialize uart
	uart_hd_init_uart(0x0); // init GPIO and Timer
	
	//main loop
	//while(1){

		for (i = 0; i < 5000000; i = i + 1)
			__NOP();

		//uart_hd_putchar(0xA5);

		/*for (i = 0; i < 200; i = i + 1)
			__NOP();*/
		
		for (i = 0; i < 9600; i++){
			uart_hd_putchar('0');
			uart_hd_putchar('1');
			uart_hd_putchar('2');
			uart_hd_putchar('3');
			uart_hd_putchar('4');
			uart_hd_putchar('5');
			uart_hd_putchar('6');
			uart_hd_putchar('7');
		}
		//uart_hd_putchar('8');
		//uart_hd_putchar('9');
		//uart_hd_putchar(0x0a);
		//uart_hd_putchar(0x0d);
		
	//}

	while(1);
   
  return 0;
}
  
