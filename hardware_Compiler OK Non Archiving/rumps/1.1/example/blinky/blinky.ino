#include <gpio.h>

#define __NOP() __asm__("nop\n\t")

// ---------------------
// Core 0
// ---------------------
void setup() {
  // put your setup code here, to run once:
  GPIO_OEN_SET = 0xffffffff;  // set all output
    
  GPIO_DATAOUT = 0x00000000;
}

void loop() {
  // put your main code here, to run repeatedly:
  int i = 0;
  
  GPIO_DATAOUT = 0x00000000;

  for (i = 0; i < 500000; i++){
    __NOP();
  }

  GPIO_DATAOUT = 0xffffffff;

  for (i = 0; i < 500000; i++){
    __NOP();
  }
}

// ---------------------
// Core 1
// ---------------------
void setup1(){
  // put your setup code here, to run once:
  GPIO_OEN_SET = 0xffffffff;  // set all output
    
  GPIO_DATAOUT = 0x00000000;
}

void loop1(){
  // put your main code here, to run repeatedly:
  int i = 0;
  
  GPIO_DATAOUT = 0x00000000;

  for (i = 0; i < 500000; i++){
    __NOP();
  }

  GPIO_DATAOUT = 0xffffffff;

  for (i = 0; i < 500000; i++){
    __NOP();
  }
}

// ---------------------
// Core 2
// ---------------------
void setup2(){
  // put your setup code here, to run once:
  GPIO_OEN_SET = 0xffffffff;  // set all output
    
  GPIO_DATAOUT = 0x00000000;
}

void loop2(){
  // put your main code here, to run repeatedly:
  int i = 0;
  
  GPIO_DATAOUT = 0x00000000;

  for (i = 0; i < 500000; i++){
    __NOP();
  }

  GPIO_DATAOUT = 0xffffffff;

  for (i = 0; i < 500000; i++){
    __NOP();
  }
}

// ---------------------
// Core 3
// ---------------------
void setup3(){
  // put your setup code here, to run once:
  GPIO_OEN_SET = 0xffffffff;  // set all output
    
  GPIO_DATAOUT = 0x00000000;
}

void loop3(){
  // put your main code here, to run repeatedly:
  int i = 0;
  
  GPIO_DATAOUT = 0x00000000;

  for (i = 0; i < 500000; i++){
    __NOP();
  }

  GPIO_DATAOUT = 0xffffffff;

  for (i = 0; i < 500000; i++){
    __NOP();
  }
}
