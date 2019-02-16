//------------------------------------------------------------------------------
//
// Main Program
// Application		: uart_hd print string
// Core						: Any
// Purpose
//	- Test uart_hd function

#include "main.h"

void main_scheduler (void);

//------------------------------------------------------------------------------
int main(void)
{
	int i = 0;
  char ch;
  
  //GPIO_OEN_SET = 0x1; //gpio 0 as output
  
  uart_hd_init_uart(0x1);
  
  while(1){
		ch = uart_hd_getchar();
		
		if(ch=='a'){
			uart_hd_print("Received ");
			uart_hd_println("A");
		}
		else if(ch=='b'){
			uart_hd_print("Received ");
			uart_hd_println("b");
		}
  }
   
  return 0;
}
  
