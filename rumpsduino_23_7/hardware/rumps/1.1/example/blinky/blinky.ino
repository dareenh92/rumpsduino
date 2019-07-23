
// ---------------------
// Core 0
// ---------------------
void setup() {
  serial_begin(115200);
  pinMode(19, INPUT);
  pinMode(18, OUTPUT);
}

void loop() {
  if(digitalRead(19)){
//  if((GPIO_DATAIN & 0x00080000) == 0x00080000){
////    GPIO_BCLR = 0x1 << 18;
////    delay(500);
    serial_println("Button pressed");
  }
  else
  {
//    GPIO_BSET = 0x1 << 18;
//    delay(500);
    serial_println("NOPE");
  }
}

// ---------------------
// Core 1
// ---------------------

void setup1() {
  pinMode(32, OUTPUT);
}

void loop1() {
  digitalWrite(32, HIGH);
  delay(1000);
  digitalWrite(32,LOW);
  delay(1000);
}

// ---------------------
// Core 2
// ---------------------

void setup2() {
  pinMode(40, OUTPUT);
}

void loop2() {
  digitalWrite(40, LOW);
  delay(500);
  digitalWrite(40,HIGH);
  delay(500);
 }

// ---------------------
// Core 3
// ---------------------

void setup3() {
    pinMode(48, OUTPUT);
  }

void loop3() {
  digitalWrite(48, LOW);
  delay(100);
  digitalWrite(48,HIGH);
  delay(100);
}
