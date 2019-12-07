
// Core 0 program
void setup()
{
  serial_begin(0x1);
}

// the loop function runs over and over again forever
void loop()
{
  serial_putchar('A');
  serial_print(" HELLO ");
  serial_println("world");
  delay(500);
}

// Core 1 program
void setup1()
{
  del1 = 200000;
  pinMode(32, OUTPUT);
}

void loop1()
{
  digitalWrite(32, HIGH);
  delayMicroseconds(del1);
  digitalWrite(32, LOW);
  delayMicroseconds(del1);
}

// Core 2 program
void setup2() {}
void loop2() {}

// Core 3 program
void setup3() {}
void loop3() {}
