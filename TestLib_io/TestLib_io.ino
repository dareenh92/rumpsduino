#include <gpio.h>

void setup() {
  // put your setup code here, to run once:
  pinMode(0, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalRead(10);
  digitalWrite(0,HIGH);
  delay(50);
  digitalWrite(0,LOW);
  delay(50);
//  millis();
//  micros();
}

// Core 2
void setup1(){
   pinMode(1, OUTPUT);
}

void loop1() {
  digitalWrite(1,HIGH);
  delay(200);
  digitalWrite(1,LOW);
  delay(200);
}

//Core 3
void setup2(){
   pinMode(2, OUTPUT);
}

void loop2() {
  digitalWrite(2,HIGH);
  delay(1000);
  digitalWrite(2,LOW);
  delay(1000);
}


//Core 3
void setup3(){
   pinMode(3, OUTPUT);
}

void loop3() {
  digitalWrite(3,HIGH);
  delay(5000);
  digitalWrite(3,LOW);
  delay(5000);
}

