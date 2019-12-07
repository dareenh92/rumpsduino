#ifndef __SERIAL_DC_H_
#define __SERIAL_DC_H_


#ifdef __cplusplus
extern "C" {
#endif
#include "Arduino.h"
#include <stdio.h>

#ifndef UART_HD_TX
#define UART_HD_TX 0x00020000 //17
#endif

#ifndef UART_HD_RX
#define UART_HD_RX 0x00010000 //16
#endif

// #ifndef UART_HD_TX
// #define UART_HD_TX 16	// GPIO 4
// #endif

// #ifndef UART_HD_RX
// #define UART_HD_RX 32   // GPIO 5
// #endif



//--------------------------------------------------------------
#define uart_hd_recv_bit(inbuf) { \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	if (GPIO_DATAIN & UART_HD_RX){ \
	  __NOP(); \
	  __NOP(); \
	  \
	  inbuf = 0x1; \
	} \
    else { \
  	  inbuf = 0x0; \
    } \
	\
}

//--------------------------------------------------------------
#define uart_hd_recv_bit115200(inbuf) { \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	__NOP(); \
	\
	if (GPIO_DATAIN & UART_HD_RX){ \
	  __NOP(); \
	  __NOP(); \
	  \
	  inbuf = 0x1; \
	} \
    else { \
  	  inbuf = 0x0; \
    } \
	\
}

#ifndef SHORTEN_UART_HD
#define uart_hd_poll_send() { \
	\
	GPIO_BCLR = UART_HD_TX;	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done\n" \
		"uhd_tx_z:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done:\n" \
			"nop" \
		:: [input0] "m" (outbuf0), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z1\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done1\n" \
		"uhd_tx_z1:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done1:\n" \
			"nop" \
		:: [input0] "m" (outbuf1), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z2\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done2\n" \
		"uhd_tx_z2:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done2:\n" \
			"nop" \
		:: [input0] "m" (outbuf2), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z3\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done3\n" \
		"uhd_tx_z3:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done3:\n" \
			"nop" \
		:: [input0] "m" (outbuf3), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z4\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done4\n" \
		"uhd_tx_z4:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done4:\n" \
			"nop" \
		:: [input0] "m" (outbuf4), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z5\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done5\n" \
		"uhd_tx_z5:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done5:\n" \
			"nop" \
		:: [input0] "m" (outbuf5), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z6\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done6\n" \
		"uhd_tx_z6:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done6:\n" \
			"nop" \
		:: [input0] "m" (outbuf6), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z7\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done7\n" \
		"uhd_tx_z7:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done7:\n" \
			"nop" \
		:: [input0] "m" (outbuf7), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
	GPIO_BSET = UART_HD_TX;	\
}
#else
#define uart_hd_poll_send() { \
	__NOP(); \
}
#endif

//------------------------------------------------
#define uart_hd_poll_send115200() { \
	volatile int i; \
	\
	GPIO_BCLR = UART_HD_TX;	\
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done_115200\n" \
		"uhd_tx_z_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf0), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z1_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done1_115200\n" \
		"uhd_tx_z1_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done1_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf1), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z2_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done2_115200\n" \
		"uhd_tx_z2_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done2_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf2), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z3_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done3_115200\n" \
		"uhd_tx_z3_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done3_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf3), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z4_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done4_115200\n" \
		"uhd_tx_z4_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done4_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf4), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z5_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done5_115200\n" \
		"uhd_tx_z5_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done5_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf5), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z6_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done6_115200\n" \
		"uhd_tx_z6_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done6_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf6), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	asm( \
			"ldr  r0, %[input0]\n" \
			"cmp  r0, #00000000\n" \
			"beq  uhd_tx_z7_115200\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input3]\n" \
			"b	 uhd_tx_done7_115200\n" \
		"uhd_tx_z7_115200:\n" \
			"ldr  r1, %[input1]\n" \
			"str  r1, %[input2]\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
			"nop\n" \
		"uhd_tx_done7_115200:\n" \
			"nop" \
		:: [input0] "m" (outbuf7), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	\
	GPIO_BSET = UART_HD_TX;	\
}


//--------------------------------------------------------------
#ifndef SHORTEN_UART_HD
#define uart_hd_poll_recv() { \
	\
	while (GPIO_DATAIN & UART_HD_RX); \
	\
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
    __NOP(); \
	__NOP(); \
	uart_hd_recv_bit(inbuf0); \
	uart_hd_recv_bit(inbuf1); \
	uart_hd_recv_bit(inbuf2); \
	uart_hd_recv_bit(inbuf3); \
	uart_hd_recv_bit(inbuf4); \
	uart_hd_recv_bit(inbuf5); \
	uart_hd_recv_bit(inbuf6); \
	uart_hd_recv_bit(inbuf7); \
    \
    uart_hd_recv_bit(stop); \
}
#else
#define uart_hd_poll_recv() { \
	__NOP(); \
}
#endif

//--------------------------------------------------------------
#define uart_hd_poll_recv115200() { \
	volatile int i; \
	\
	while (GPIO_DATAIN & UART_HD_RX); \
	\
	GPIO_BTGL = 0x00000004; \
	\
	for(i = 0; i < 3; i++) \
	  __NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	uart_hd_recv_bit115200(inbuf0); \
	uart_hd_recv_bit115200(inbuf1); \
	uart_hd_recv_bit115200(inbuf2); \
	uart_hd_recv_bit115200(inbuf3); \
	uart_hd_recv_bit115200(inbuf4); \
	uart_hd_recv_bit115200(inbuf5); \
	uart_hd_recv_bit115200(inbuf6); \
	uart_hd_recv_bit115200(inbuf7); \
    \
    uart_hd_recv_bit115200(stop); \
}



////////////////////////////
// Generic Function
////////////////////////////
//void uart_hd_timer_isr(void);
void serial_begin(int baud_div);
void serial_putchar( char ch );
char serial_getchar( );
void serial_print(const char* print_string);
void serial_print_num(uint32_t number);
void serial_println(const char* print_string);
char* serial_read( );


#ifdef __cplusplus
}
#endif

#endif
