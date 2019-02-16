//------------------------------------------------------------------------------
//
#include "main.h"


/*memory access functions
 */


void set_aes_key(unsigned int k3, unsigned int k2, unsigned int k1, unsigned int k0)
{
	AES_KEY0 = k0;
	AES_KEY1 = k1;
	AES_KEY2 = k2;
	AES_KEY3 = k3;
}

void put_plain(unsigned int p3, unsigned int p2, unsigned int p1, unsigned int p0)
{
	AES_PLAIN0 = p0;
	AES_PLAIN1 = p1;
	AES_PLAIN2 = p2;
	AES_PLAIN3 = p3;
}

int compare_cipher(unsigned int c3, unsigned int c2, unsigned int c1, unsigned int c0)
{
	if(AES_CIPHER0 != c0)
		return 0;
	if(AES_CIPHER1 != c1)
		return 1;
	if(AES_CIPHER2 != c2)
		return 2;
	if(AES_CIPHER3 != c3)
		return 3;

	return 4;
}
