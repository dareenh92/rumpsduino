#ifndef __MATRIX_H_
#define __MATRIX_H_

 
#include <stdio.h>
 
#include "type.h"

#define MX_CLK  0x10
#define MX_CS   0x20
#define MX_DIN  0x40


u8 disp1[36][8];

void Write_MX_byte(u8 DATA);

void Write_MX(u8 address, u8 dat);

void Init_MX(void);

#endif
