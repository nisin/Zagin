#include <RotaryEncoder.h>

RotaryEncoder::RotaryEncoder(uint8_t pinA, uint8_t pinB, uint8_t cog)
{
    this->pinA = pinA;
    this->pinB = pinB;
    this->cog = cog;
    pinMode(pinA,INPUT_PULLUP);
    pinMode(pinB,INPUT_PULLUP);
}

RotaryEncoder::~RotaryEncoder()
{
}


int RotaryEncoder::read() {
  int val_a = LOW;
  int val_b = LOW;
  int cnt = 0;
  int a,b;
  while (cnt<5) {
    if (((a = digitalRead(pinA)) == val_a) && ((b = digitalRead(pinB)) == val_b ))
      ++cnt;
    else
      cnt=0;
    val_a = a;
    val_b = b;
    delayMicroseconds(1);
  }
  uint8_t rot = ((last << 2) + (val_a << 1) + val_b) & 0xf;
  //  last              00 00 00 00 01 01 01 01 10 10 10 10 11 11 11 11
  //  current           00 01 10 11 00 01 10 11 00 01 10 11 00 01 10 11
  //  value              0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15
  //  error/equal        =        E     =  E        E  =     E        =
  static const int d[]={ 0,-1, 1, 0, 1, 0, 0,-1,-1, 0, 0, 1, 0, 1,-1, 0 };
  last = rot & 0b11;
  cogcnt += d[rot];
  if (abs(cogcnt) >= cog) {
    cogcnt = 0;
    return d[rot];
  }
  return 0;
}

void RotaryEncoder::flash() {
  last = 0;
  cogcnt=0;
}
