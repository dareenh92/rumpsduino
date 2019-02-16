#include <gpio.h>

#define __NOP() __asm__("nop\n\t")

void setup() {
  // put your setup code here, to run once:
  GPIO_OEN_SET = 0xffffffff;  // set all output
    
  GPIO_DATAOUT = 0x00000000;
}

void loop() {
  // put your main code here, to run repeatedly:
  int i = 0;
  
  GPIO_DATAOUT = 0x00000000;

  for (i = 0; i < 100000; i++){
    __NOP();
  }

  GPIO_DATAOUT = 0xffffffff;

  for (i = 0; i < 100000; i++){
    __NOP();
  }
  
  /*GPIO_DATAOUT = 0x00000000;

  for (i = 0; i < 300000; i++){
    __NOP();
  }

  GPIO_DATAOUT = 0xffffffff;

  for (i = 0; i < 300000; i++){
    __NOP();
  }*/
  
  /*GPIO_DATAOUT = 0x00000000;

  for (i = 0; i < 900000; i++){
    __NOP();
  }

  GPIO_DATAOUT = 0xffffffff;

  for (i = 0; i < 900000; i++){
    __NOP();
  }*/
}
