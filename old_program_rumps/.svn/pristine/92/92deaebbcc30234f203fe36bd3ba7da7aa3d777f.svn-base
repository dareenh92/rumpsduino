//------------------------------------------------------------------------------
//
// Main Program
// Application		: floatpoint
// Core						: Any
// Purpose
//	- Testing float point calculation on M0

#include "main.h"
void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
	volatile float f = 57.8;
	volatile float temp_f;
	
	volatile int int_part;
	volatile int dec_part;
	
	volatile unsigned char _command;
	
	//initialize uart
	init_uart();
	
	//main loop
	while(1){
		_command = _getchar();
		_putchar(_command);
		
		switch(_command){
			case 'a':
				temp_f = f+0.9;
				break;
				
			case 'b':
				temp_f = f*2;
				break;
				
			case 'c':
				temp_f = f/3;
				break;
		}
		
		int_part = (int)temp_f;
		dec_part = (int)( (temp_f - int_part) * (100) + 0.5);
		_putchar(int_part);
		_putchar(dec_part);
	}
	
	return 0;
}
  

