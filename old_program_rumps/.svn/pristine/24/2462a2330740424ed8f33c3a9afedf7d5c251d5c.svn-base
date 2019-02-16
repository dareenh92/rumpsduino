#ifndef __MEMORY_H_
#define __MEMORY_H_

#include <stdio.h>

#define memory  ((*(volatile unsigned long *)(0x20000000)))
#define shared_memory  ((*(volatile unsigned long *)(0x20002000)))

void write8(unsigned int addr, unsigned char data);
void write16(unsigned int addr, unsigned short data);
void write32(unsigned int addr, unsigned int data);
unsigned char read8(unsigned int addr);
unsigned short read16(unsigned int addr);
unsigned int read32(unsigned int addr);

void write_shared32(unsigned int addr, unsigned int data);
unsigned int read_shared32(unsigned int addr);

#endif


