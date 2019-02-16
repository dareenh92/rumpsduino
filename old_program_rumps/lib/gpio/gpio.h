#ifndef __GPIO_H_
#define __GPIO_H_

#include <stdio.h>


#define GPIO_DATAIN             ((*(volatile unsigned long *) (0x50000000)))
#define GPIO_DATAOUT            ((*(volatile unsigned long *) (0x50000004)))
#define GPIO_OEN_SET            ((*(volatile unsigned long *) (0x50000008)))
#define GPIO_OEN_CLR            ((*(volatile unsigned long *) (0x5000000C)))
#define GPIO_IF                 ((*(volatile unsigned long *) (0x50000010)))
#define GPIO_IE_SET             ((*(volatile unsigned long *) (0x50000014)))
#define GPIO_IE_CLR             ((*(volatile unsigned long *) (0x50000018)))
#define GPIO_IT_SET             ((*(volatile unsigned long *) (0x5000001C)))
#define GPIO_IT_CLR             ((*(volatile unsigned long *) (0x50000020)))
#define GPIO_IP_SET             ((*(volatile unsigned long *) (0x50000024)))
#define GPIO_IP_CLR             ((*(volatile unsigned long *) (0x50000028)))
#define GPIO_BSET               ((*(volatile unsigned long *) (0x5000002C)))
#define GPIO_BCLR               ((*(volatile unsigned long *) (0x50000030)))
#define GPIO_BTGL               ((*(volatile unsigned long *) (0x50000034)))

#define LOW 0
#define HIGH 1
#define INPUT 0
#define OUTPUT 1

#define pinMode_output(pin_num) GPIO_OEN_SET = 0x1 << pin_num;
#define pinMode_input(pin_num) GPIO_OEN_CLR = 0x1 << pin_num;
#define digitalWrite_high(pin_num) GPIO_BSET = 0x1 << pin_num;
#define digitalWrite_low(pin_num) GPIO_BCLR = 0x1 << pin_num;
#define digitalRead_pin(pin_num) ((GPIO_DATAIN >> pin_num) & 0x1);

#endif


