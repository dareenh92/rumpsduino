#include <gpio.h>

//*******
// Core 0
//*******
const int core0_out = 18;
const int core0_in = 19;
void setup()
{
  pinMode(core0_out, OUTPUT);
}

void loop()
{
  digitalWrite(core0_out, HIGH);
  delay(50);
  digitalWrite(core0_out, LOW);
  delay(50);
}

//*******
// Core 1
//*******
const int core1_out = 0;
const int core1_in = 2;
void setup1()
{
  pinMode(core1_out, OUTPUT);
}

void loop1()
{
  digitalWrite(core1_out, HIGH);
  delay(500);
  digitalWrite(core1_out, LOW);
  delay(500);
}

//*******
//Core 2
//*******
const int core2_out = 0;
const int core2_in = 2;
void setup2()
{
  pinMode(core2_out, OUTPUT);
  pinMode(core2_in, INPUT);
}

void loop2()
{
  while (digitalRead(core2_in) == LOW)
    digitalWrite(core2_out, HIGH);

  digitalWrite(core2_out, LOW);
}

//*******
//Core 3
//*******
const int core3_out = 0;
const int core3_in = 2;
void setup3()
{
  pinMode(core3_out, OUTPUT);
  pinMode(core3_in, INPUT);
}

void loop3()
{
  while (digitalRead(core3_in) == LOW)
    digitalWrite(core3_out, LOW);

  digitalWrite(core3_out, HIGH);
}