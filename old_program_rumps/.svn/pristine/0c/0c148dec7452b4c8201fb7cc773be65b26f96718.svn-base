//------------------------------------------------------------------------------
//
#include "main.h"

unsigned char tm_ocr_temp;

volatile short qin = 0;
volatile short qout = 0;

static char UART_HD_BAUD;

////////////////////////////////
// Interface routines required:
// (Platform dependent)
////////////////////////////////

 
//-----------------------------------------------------------------------
// IOC: 0x0 => Normal core / DSP core
//		  0x1 => IO-control core
// mode: 0x0 => TX
//			 0x1 => RX
void uart_hd_init_uart( char baud_div ){
	UART_HD_BAUD = baud_div;

	GPIO_BSET = UART_HD_TX;		// TX high during IDLE
	
	GPIO_OEN_CLR = UART_HD_RX;	// set GPIO pin as input (RX)
	GPIO_OEN_SET = UART_HD_TX;	// set GPIO pin as output (TX)
	
} //ENDFUNCTION: init_uart
 

//-----------------------------------------------------------------------
void uart_hd_putchar( char ch ){

	volatile unsigned int outbuf0;
	volatile unsigned int outbuf1;
	volatile unsigned int outbuf2;
	volatile unsigned int outbuf3;

	volatile unsigned int outbuf4;
	volatile unsigned int outbuf5;
	volatile unsigned int outbuf6;
	volatile unsigned int outbuf7;

	volatile unsigned int stop;

	volatile unsigned int tx;

	outbuf0 = (ch & 0x1);
	outbuf1 = (ch & 0x2);
	outbuf2 = (ch & 0x4);
	outbuf3 = (ch & 0x8);

	outbuf4 = (ch & 0x10);
	outbuf5 = (ch & 0x20);
	outbuf6 = (ch & 0x40);
	outbuf7 = (ch & 0x80);

	stop = 0x1;

	tx = UART_HD_TX;
 
	//invoke_UART_transmit
	if (UART_HD_BAUD == 0){
	  uart_hd_poll_send();
	}
	else{
	  uart_hd_poll_send115200();
	}
	/*asm(
			"ldr  r0, %[input0]\n"
			"cmp  r0, #00000000\n"
			"beq  uhd_tx_z\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"ldr  r1, %[input1]\n"
			"str  r1, %[input3]\n"
			"b	 uhd_tx_done\n"
			"uhd_tx_z:\n"
			"ldr  r1, %[input1]\n"
			"str  r1, %[input2]\n"
			"nop\n"
			"nop\n"
			"nop\n"
			"uhd_tx_done:\n"
			"nop"
		:: [input0] "m" (outbuf0), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :);*/
}


//-----------------------------------------------------------------------
char uart_hd_getchar( ){

	unsigned char inbuf;

	volatile unsigned char inbuf0;
	volatile unsigned char inbuf1;
	volatile unsigned char inbuf2;
	volatile unsigned char inbuf3;

	volatile unsigned char inbuf4;
	volatile unsigned char inbuf5;
	volatile unsigned char inbuf6;
	volatile unsigned char inbuf7;

	volatile unsigned char stop;

	// invoke_UART_receive
	if (UART_HD_BAUD == 0){
	  uart_hd_poll_recv();
	}
	else{
	  uart_hd_poll_recv115200();
	}

	inbuf = inbuf0;
	inbuf |= (inbuf1 << 1);
	inbuf |= (inbuf2 << 2);
	inbuf |= (inbuf3 << 3);
	inbuf |= (inbuf4 << 4);
	inbuf |= (inbuf5 << 5);
	inbuf |= (inbuf6 << 6);
	inbuf |= (inbuf7 << 7);
    	
    return inbuf;
}

void uart_hd_print(const char* print_string){
	int i=0;
	
	while(print_string[i]!='\0')
		uart_hd_putchar(print_string[i++]); //put string
}

void uart_hd_println(const char* print_string){
	int i=0;
	
	while(print_string[i]!='\0')
		uart_hd_putchar(print_string[i++]); //put string
		
	uart_hd_putchar(0xa); //put newline character
	uart_hd_putchar(0xd); //put carriage return character
}
