//------------------------------------------------------------------------------
//
#include "main.h"


/*memory access functions
 */


void write32(unsigned int addr, unsigned int data)
{
	unsigned char *ptr8;

	unsigned int *ptr32;

	if (addr % 4 == 0){
			ptr8 = (unsigned char *) &memory;
			ptr8 = ptr8 + addr;
			ptr32 = (unsigned int *) ptr8;
			*ptr32 = data;
	}

	return;
}

void write16(unsigned int addr, unsigned short data)
{
	unsigned char *ptr8;

	unsigned short *ptr16;

	if (addr % 2 == 0){
		ptr8 = (unsigned char *) &memory;
		ptr8 = ptr8 + addr;
		ptr16 = (unsigned short *) ptr8;
		*ptr16 = data;
	}

	return;
}

void write8(unsigned int addr, unsigned char data)
{
	unsigned char *ptr8;

	ptr8 = (unsigned char *) &memory;
	ptr8 = ptr8 + addr;
	*ptr8 = data;

	return;
}


unsigned int read32(unsigned int addr)
{
	unsigned char *ptr8;

	unsigned int *ptr32;

	if (addr % 4 == 0){
			ptr8 = (unsigned char *) &memory;
			ptr8 = ptr8 + addr;
			ptr32 = (unsigned int *) ptr8;

			return *ptr32;
	}

	return 0;
}

unsigned short read16(unsigned int addr)
{
	unsigned char *ptr8;

	unsigned short *ptr16;

	if (addr % 2 == 0){
		ptr8 = (unsigned char *) &memory;
		ptr8 = ptr8 + addr;
		ptr16 = (unsigned short *) ptr8;

		return *ptr16;
	}

	return 0;
}

unsigned char read8(unsigned int addr)
{
	unsigned char *ptr8;

	ptr8 = (unsigned char *) &memory;
	ptr8 = ptr8 + addr;

	return *ptr8;
}




void write_shared32(unsigned int addr, unsigned int data)
{
	unsigned char *ptr8;

	unsigned int *ptr32;

	if (addr % 4 == 0){
			ptr8 = (unsigned char *) &shared_memory;
			ptr8 = ptr8 + addr;
			ptr32 = (unsigned int *) ptr8;
			*ptr32 = data;
	}

	return;
}


unsigned int read_shared32(unsigned int addr)
{
	unsigned char *ptr8;

	unsigned int *ptr32;

	if (addr % 4 == 0){
			ptr8 = (unsigned char *) &shared_memory;
			ptr8 = ptr8 + addr;
			ptr32 = (unsigned int *) ptr8;

			return *ptr32;
	}

	return 0;
}

