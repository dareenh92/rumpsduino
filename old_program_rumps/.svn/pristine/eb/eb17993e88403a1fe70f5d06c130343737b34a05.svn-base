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
	char ch;
	
	//initialize uart
	init_uart();
	
	//main loop
	while(1){
		
		ch = _getchar();
		_putchar(ch);
		
		switch(ch){
			case 'a':
				_putchar('1');
				_putchar('2');
				_putchar('3');
				break;
				
			case 'b':
				_putchar('4');
				_putchar('5');
				_putchar('6');
				break;
				
			case 'c':
				_putchar('7');
				_putchar('8');
				_putchar('9');
				break;
		}
		
		_putchar('#');
		
	}
	
	return 0;

}
