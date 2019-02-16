#ifndef __PP_H_
#define __PP_H_

#include <stdio.h>

#define PP_BUFF 	((*(volatile unsigned long *)	(0x50030000)))
#define PP_ST	  	((*(volatile unsigned long *)	(0x50030004)))

#endif


