#ifndef __UART_DC_H_
#define __UART_DC_H_


#include <stdio.h>
 
//#define BAUD_RATE       230400.0


#ifndef UART_HD_TX
#define UART_HD_TX 0x01	// GPIO 0
#endif

#ifndef UART_HD_RX
#define UART_HD_RX 0x02 // GPIO 1
#endif

/*
//--------------------------------------------------------------
#define uart_hd_send_bit(outbuf) { \
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
	if (outbuf) { \
	  \
	  __NOP(); \
	  __NOP(); \
	  \
	  GPIO_BSET = UART_HD_TX; \
	} \
	else { \
	  GPIO_BCLR = UART_HD_TX; \
	} \
	\
}*/

/*
//--------------------------------------------------------------
#define uart_hd_send_bit(outbuf) { \
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
	__ASM{ \
			ldrb r0, outbuf; \
			ldrb r1, 0x00; \
			ldrb r2, UART_HD_TX; \
			cmp  r0, r1; \
			beq  uhd_tx_z; \
			strb r2, GPIO_BSET; \
			b	 uhd_tx_done; \
        uhd_tx_z: \
		    strb r2, GPIO_BCLR; \
		uhd_tx_done: \
			nop; \
	} \
	  \
	  __NOP(); \
	  __NOP(); \
	  \
	  GPIO_BSET = UART_HD_TX; \
	} \
	else { \
	  GPIO_BCLR = UART_HD_TX; \
	} \
	\
}
*/
//--------------------------------------------------------------
/*#define uart_hd_send_bit(outbuf, uhd_tx_z, uhd_tx_done) { \
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
		"uhd_tx_done:\n" \
			"nop" \
		:: [input0] "m" (outbuf), [input1] "m" (tx), [input2] "m" (GPIO_BCLR) , [input3] "m" (GPIO_BSET) :); \
}*/


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

/*
//--------------------------------------------------------------
#define uart_hd_poll_send() { \
	\
	GPIO_BCLR = UART_HD_TX;	\
	__NOP(); \
	__NOP(); \
	__NOP(); \
	uart_hd_send_bit(outbuf0, uhd_tx_z0, uhd_tx_done0); \
	uart_hd_send_bit(outbuf1, uhd_tx_z1, uhd_tx_done1); \
	uart_hd_send_bit(outbuf2, uhd_tx_z2, uhd_tx_done2); \
	uart_hd_send_bit(outbuf3, uhd_tx_z3, uhd_tx_done3); \
	uart_hd_send_bit(outbuf4, uhd_tx_z4, uhd_tx_done4); \
	uart_hd_send_bit(outbuf5, uhd_tx_z5, uhd_tx_done5); \
	uart_hd_send_bit(outbuf6, uhd_tx_z6, uhd_tx_done6); \
	uart_hd_send_bit(outbuf7, uhd_tx_z7, uhd_tx_done7); \
    \
	uart_hd_send_bit(stop, uhd_tx_z_s, uhd_tx_done_s); \
	__NOP(); \
	__NOP(); \
	__NOP(); \
	\
}*/

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
void uart_hd_init_uart( char baud_div );
void uart_hd_putchar( char ch );
char uart_hd_getchar( );
void uart_hd_print(const char* print_string);
void uart_hd_println(const char* print_string);

#endif
