int del1;
int tes;
void setup() {
  serial_begin(0x1); 
}

// the loop function runs over and over again forever
void loop() {
    
//   tes=serial_getchar();
  serial_putchar('A');
  serial_print(" HALO ");
  serial_println("Apakabar");
  delay(500); 
}

void setup1(){
  del1=200000;
  pinMode(32, OUTPUT);
}

void loop1(){
  digitalWrite(32, HIGH);
  delayMicroseconds(del1);
  digitalWrite(32,LOW);
  delayMicroseconds(del1);
}

void setup2(){}
void loop2(){}

void setup3(){}
void loop3(){}
