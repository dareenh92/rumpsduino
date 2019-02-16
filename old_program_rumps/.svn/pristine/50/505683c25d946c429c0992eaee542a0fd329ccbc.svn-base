//------------------------------------------------------------------------------

//

// Main Program
// Application		: map_algo
// Core				: Any
// Purpose
//	- Testing MAP decoding algorithm on M0


#include "main.h"

void main_scheduler (void);

//------------------------------------------------------------------------------

int main(void){
	
	// to store data received through serial
	char ch, ch0, ch1, ch2, ch3;
	int i;
	
	//initialize uart
	uart_hd_init_uart(0x1); // init GPIO
	
	GPIO_OEN_SET = 0x00000004;	// set all output

	GPIO_BSET = 0x00000004;



	//main loop
	//while(1){

	//for (i = 0; i < 5000000; i = i + 1)
	//	__NOP();

	//uart_hd_putchar(0xA5);

	/*for (i = 0; i < 200; i = i + 1)
		__NOP();*/

	for (i = 0; i < 2000000; i = i + 1)
	  __NOP();

	//ch0 = uart_hd_getchar(0x1);

	uart_hd_putchar(0x55);

	for (i = 0; i < 2000000; i = i + 1)
	  __NOP();

	uart_hd_putchar(0x75);

	uart_hd_putchar(0x58);

	uart_hd_putchar(0xaa);
		
	/*for (i = 0; i < 9600; i++){
		uart_hd_putchar('0');
		uart_hd_putchar('1');
		uart_hd_putchar('2');
		uart_hd_putchar('3');
		uart_hd_putchar('4');
		uart_hd_putchar('5');
		uart_hd_putchar('6');
		uart_hd_putchar('7');
	}*/
	//uart_hd_putchar('8');
	//uart_hd_putchar('9');
	//uart_hd_putchar(0x0a);
	//uart_hd_putchar(0x0d);
		
	//}

	//while(1);

	ch0 = uart_hd_getchar();
	ch1 = uart_hd_getchar();
	ch2 = uart_hd_getchar();
	ch3 = uart_hd_getchar();

	uart_hd_putchar(0x12);
	uart_hd_putchar(ch0);
	uart_hd_putchar(ch1);
	uart_hd_putchar(ch2);
	uart_hd_putchar(ch3);
	uart_hd_putchar(0xef);

	while(1)
	{
	  GPIO_BCLR = 0x00000004;

	  for (i = 0; i < 100000; i++)
	    __NOP();

	  GPIO_BSET = 0x00000004;

	  for (i = 0; i < 100000; i++)
	    __NOP();
	}
	
	return 0;

}
