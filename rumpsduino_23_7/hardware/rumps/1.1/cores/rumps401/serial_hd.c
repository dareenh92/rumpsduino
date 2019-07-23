//------------------------------------------------------------------------------
//
#include "serial_hd.h"
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

static int UART_HD_BAUD;
////////////////////////////////
// Interface routines required:
// (Platform dependent)
////////////////////////////////

 
//-----------------------------------------------------------------------
// IOC: 0x0 => Normal core / DSP core
//		  0x1 => IO-control core
// mode: 0x0 => TX
//			 0x1 => RX
void serial_begin( int baud_div){
	UART_HD_BAUD = baud_div;

	GPIO_BSET = UART_HD_TX;		// TX high during IDLE

	GPIO_OEN_CLR = UART_HD_RX;	// set GPIO pin as input (RX)
	GPIO_OEN_SET = UART_HD_TX;	// set GPIO pin as output (TX)
	
} //ENDFUNCTION: init_uart
 

//-----------------------------------------------------------------------
void serial_putchar( char ch ){

	unsigned int outbuf0;
	unsigned int outbuf1;
	unsigned int outbuf2;
	unsigned int outbuf3;	
	unsigned int outbuf4;
	unsigned int outbuf5;
	unsigned int outbuf6;
	unsigned int outbuf7;

	unsigned int stop;

	unsigned int tx;

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
}


//-----------------------------------------------------------------------
char serial_getchar( ){

	unsigned char inbuf;

	unsigned char inbuf0;
	unsigned char inbuf1;
	unsigned char inbuf2;
	unsigned char inbuf3;

	unsigned char inbuf4;
	unsigned char inbuf5;
	unsigned char inbuf6;
	unsigned char inbuf7;

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

void serial_print(const char* print_string){
	int i=0;
	
	while(print_string[i]!='\0')
		serial_putchar(print_string[i++]); //put string
}

// void serial_print_num(double n){
//   double m = 0;
//   for(n; n; n /= 10) m = m * 10 + n % 10;
//   for(m; m; m /= 10) serial_putchar('0' + (m % 10));

// }

void serial_print_num(uint32_t n){
  if(n>9) serial_print_num(n/10);
  serial_putchar('0' + (n % 10));
}

void serial_println(const char* print_string){
	int i=0;
	
	while(print_string[i]!='\0')
		serial_putchar(print_string[i++]); //put string
		
	serial_putchar(0xa); //put newline character
	serial_putchar(0xd); //put carriage return character
}

char* serial_read(){
	int i=0;
	static char str[1000];
	memset(str, 0, sizeof(str));
	while ( ( str[i] = serial_getchar() ) != '\n' )i++ ; //SEMENTARA BATASNYA ENTER

	return str;
}

#ifdef __cplusplus
}
#endif