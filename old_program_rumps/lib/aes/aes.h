#ifndef __AES_H_
#define __AES_H_

#include <stdio.h>

#define AES_CSR 	  ((*(volatile unsigned long *)	(0x50030000)))
#define AES_CSR0  	((*(volatile unsigned char *)	(0x50030000)))
#define AES_CSR1  	((*(volatile unsigned char *)	(0x50030001)))
#define AES_CSR2  	((*(volatile unsigned char *)	(0x50030002)))
#define AES_CSR3  	((*(volatile unsigned char *)	(0x50030003)))
#define AES_KEY0 	  ((*(volatile unsigned long *)	(0x50030004)))
#define AES_KEY1 	  ((*(volatile unsigned long *)	(0x50030008)))
#define AES_KEY2 	  ((*(volatile unsigned long *)	(0x5003000c)))
#define AES_KEY3 	  ((*(volatile unsigned long *)	(0x50030010)))
#define AES_PLAIN0 	((*(volatile unsigned long *)	(0x50030014)))
#define AES_PLAIN1 	((*(volatile unsigned long *)	(0x50030018)))
#define AES_PLAIN2 	((*(volatile unsigned long *)	(0x5003001c)))
#define AES_PLAIN3 	((*(volatile unsigned long *)	(0x50030020)))
#define AES_CIPHER0 ((*(volatile unsigned long *)	(0x50030024)))
#define AES_CIPHER1 ((*(volatile unsigned long *)	(0x50030028)))
#define AES_CIPHER2 ((*(volatile unsigned long *)	(0x5003002c)))
#define AES_CIPHER3 ((*(volatile unsigned long *)	(0x50030030)))


void set_aes_key(unsigned int k3, unsigned int k2, unsigned int k1, unsigned int k0);
void put_plain(unsigned int p3, unsigned int p2, unsigned int p1, unsigned int p0);
int compare_cipher(unsigned int c3, unsigned int c2, unsigned int c1, unsigned int c0);

#endif


