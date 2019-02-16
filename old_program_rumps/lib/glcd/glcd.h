#ifndef __GLCD_H_
#define __GLCD_H_

// GLCD.C
//
 
#include <stdio.h>
 
#include "type.h"

#define LCD_W 240
#define LCD_H 320

#define Bus_16        

#define LCD_RS     0x00100000    //
#define LCD_WR     0x00200000    //
#define LCD_RD     0x00400000    //
#define LCD_CS     0x00800000    //
#define LCD_REST   0x01000000    //

#define LCD_DATA   0x0001fffe    //

#define LCD_DATA_OFFSET   1    //

#define LCD_REG_WR_DATA   0x01700000      // REST = 1, CS = 0, RD = 1, WR = 1, RS = 1
#define LCD_REG_WR_REG    0x01600000      // REST = 1, CS = 0, RD = 1, WR = 1, RS = 0

//---------------------------------------------------
extern  u16 BACK_COLOR, POINT_COLOR;   //


//---------------------------------------------
#define LCD_WR_DATA(da) \
                { \
                  GPIO_BSET = LCD_RS; \
                  \
                  GPIO_DATAOUT = (LCD_REG_WR_DATA | (da << LCD_DATA_OFFSET)); \
                  \
                  GPIO_BCLR  = LCD_WR; \
                  GPIO_BSET  = LCD_WR; \
                }


//---------------------------------------------
#define LCD_WR_REG(da) \
                {  \
                  GPIO_BCLR = LCD_RS; \
                  \
                  GPIO_DATAOUT = (LCD_REG_WR_REG | (da << LCD_DATA_OFFSET));;  \
                  \
                  GPIO_BCLR  = LCD_WR; \
                  GPIO_BSET  = LCD_WR; \
                }


//---------------------------------------------------
void Lcd_Init(void); 
void LCD_Clear(u16 Color);
void Address_set(u16 x1, u16 y1, u16 x2, u16 y2);
//void LCD_WR_DATA(u16 da);
//void LCD_WR_REG(u16 da);

void LCD_DrawPoint(u16 x,u16 y, u16 color);//
void LCD_DrawPoint_big(u16 x,u16 y, u16 color);//
u16  LCD_ReadPoint(u16 x,u16 y); //
void LCD_DrawCircle(u16 x0,u16 y0,u16 r, u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode, u16 color, u16 b_color);//
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len, u16 color, u16 b_color);//
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len, u16 color, u16 b_color);//
void LCD_ShowString(u16 x,u16 y,const u8 *p, u16 color, u16 b_color);     //

void LCD_DrawStar(u16 x, u16 y, u16 color, u16 b_color);
void LCD_DrawStar24(u16 x, u16 y, u16 color, u16 b_color);
void LCD_DrawThunder(u16 x, u16 y, u16 color, u16 b_color);
 
void showhanzi(u32 x, u32 y, u8 index);


//---------------------------------------------------
//????
#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN       0XBC40 //
#define BRRED       0XFC07 //
#define GRAY        0X8430 //
//

#define DARKBLUE    0X01CF  //
#define LIGHTBLUE   0X7D7C  //
#define GRAYBLUE    0X5458 //
//
 
#define LIGHTGREEN 	0X841F //
#define LGRAY       0XC618 //

#define LGRAYBLUE   0XA651 //
#define LBBLUE      0X2B12 //


extern u8 hanzi[256];

extern u8 asc2_1608[1520];

extern u8 star_code[128];

extern u8 star_code24[72];

extern u8 thunder_code[96];


#endif
