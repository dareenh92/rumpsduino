//*******
// Core 0 program
//*******
void setup()
{
  serial_begin(0x1); //baud rate of 115200
}

// the loop function runs over and over again forever
void loop()
{
  // wait for character 'j'
  while(serial_getchar()!='j');

  // sends message
  serial_putchar('A');
  serial_print(" hello ");
  serial_println("world");
}

//*******
// Core 1 program
//*******
unsigned int core1_delay;
const int core1_led = 0;
void setup1()
{
  core1_delay = 200000;
  pinMode(core1_led, OUTPUT);
  digitalWrite(core1_led, HIGH);
}

void loop1()
{
  digitalWrite(core1_led, !digitalRead(core1_led));
  delayMicroseconds(core1_delay);
}

//*******
// Core 2 program
//*******
void setup2() {}
void loop2() {}

//*******
// Core 3 program
//*******
void setup3() {}
void loop3() {}
