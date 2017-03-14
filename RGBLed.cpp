
#include "RGBLed.h"


RGBLed::RGBLed(uint8_t rPin, uint8_t gPin, uint8_t bPin, boolean glareHigh)
{
    this->rPin = rPin;
    this->gPin = gPin;
    this->bPin = bPin;
    this->glareHigh = glareHigh;
    pinMode(rPin,OUTPUT);
    pinMode(gPin,OUTPUT);
    pinMode(bPin,OUTPUT);
    off();
}

RGBLed RGBLed::createGlareHigh(uint8_t rPin, uint8_t gPin, uint8_t bPin) {
    return RGBLed(rPin,gPin,bPin,true);
}

RGBLed RGBLed::createGlareLow(uint8_t rPin, uint8_t gPin, uint8_t bPin) {
    return RGBLed(rPin,gPin,bPin,false);
}
RGBLed::r_off() {
    digitalWrite(rPin,offValue());
    val.r=0;
}
RGBLed::g_off() {
    digitalWrite(gPin,offValue());
    val.g=0;
}
RGBLed::b_off() {
    digitalWrite(bPin,offValue());
    val.b=0;
}

RGBLed::off() { r_off(); g_off(); b_off(); }

RGBLed::r_on() {
    digitalWrite(rPin,onValue());
    val.r=UINT8_MAX;
}
RGBLed::g_on() {
    digitalWrite(gPin,onValue());
    val.g=UINT8_MAX;
}
RGBLed::b_on() {
    digitalWrite(bPin,onValue());
    val.b=UINT8_MAX;
}

RGBLed::white() { r_on();  g_on();  b_on();  }
RGBLed::red()   { r_on();  g_off(); b_off(); }
RGBLed::green() { r_off(); g_on();  b_off(); }
RGBLed::blue()  { r_off(); g_off(); b_on();  }
RGBLed::cyan()  { r_off(); g_on();  b_on();  }
RGBLed::magenta(){ r_on();  g_off(); b_on();  }
RGBLed::yellow(){ r_on();  g_on();  b_off(); }

RGBLed::rgb(uint8_t rv, uint8_t gv, uint8_t bv ) {
    r(rv); g(gv); b(bv);
}
RGBLed::r(uint8_t r) {
    analogWrite(rPin,value(r));
    val.r=r;
} 
RGBLed::g(uint8_t g) {
    analogWrite(gPin,value(g));
    val.g=g;
}
RGBLed::b(uint8_t b) {
    analogWrite(bPin,value(b));
    val.b=b;
}

HSV RGBLed::hsv() {
    int r = val.r;
    int g = val.g;
    int b = val.b;
    int max = max(r,max(g,b));
    int min = min(r,min(g,b));
    HSV hsv = {0,0,0};
    hsv.h = max - min;
    if (hsv.h > 0) {
        if (max == r) 
            hsv.h = (60 * (g - b)) / hsv.h;
        else if (max == g) 
            hsv.h = ((60 * (b - r)) / hsv.h) + 120;
        else 
            hsv.h = ((60 * (r - g)) / hsv.h) + 240;
    }
    if (hsv.h < 0)   hsv.h += 360;
    if (hsv.h > 360) hsv.h -= 360;
    
    hsv.s = (max - min);
    if (max > 0)
        hsv.s = ((unsigned int)hsv.s * UINT8_MAX) / max;
    hsv.v = max;
    return hsv;
}

RGBLed::hsv(int hue, uint8_t sat, uint8_t va) {
    int r ;
    int g ;
    int b ;
    if (sat > 0) {
        int i = hue / 60;
        int min = va - (((unsigned int)sat * va) / UINT8_MAX);  
        switch (i) {
            default:
            case 0: // 0 - 59
                r = va;
                g = ((unsigned int)(hue) * (va - min)) / 60 + min;
                b = min;
                break;
            case 5: // 300 - 360
                r = va;
                g = min;
                b = ((unsigned int)(360 - hue) * (va - min)) / 60 + min;
                break;
            case 1: // 60 - 119
                r = ((unsigned int)(120 - hue) * (va - min)) / 60 + min;
                g = va;
                b = min;
                break;
            case 2: // 120 - 180 
                r = min;
                g = va;
                b = ((unsigned int)(hue - 120) * (va - min)) / 60 + min;
                break;
            case 3: // 180 - 240
                r = min;
                g = ((unsigned int)(240 - hue) * (va - min)) / 60 + min;
                b = va;
                break;
            case 4: // 240 - 300
                r = ((hue - 240) * (va - min)) / 60 + min;
                g = min;
                b = va;
                break;
        }
    }
    rgb(r,g,b);
}

RGBLed::h(int hue) {
    HSV hsv = this->hsv();
    this->hsv(hue,(uint8_t)hsv.s,(uint8_t)hsv.v);
}
RGBLed::s(uint8_t sat) {
    HSV hsv = this->hsv();
    this->hsv(hsv.h,sat,(uint8_t)hsv.v);
}
RGBLed::v(uint8_t va) {
    HSV hsv = this->hsv();
    this->hsv(hsv.h,(uint8_t)hsv.s,va);
}
RGBLed::rotateHue(int amount) {
    HSV hsv = this->hsv();
    hsv.h = hsv.h + amount;
    if (hsv.h < 0) 
        hsv.h = (hsv.h % 360) + 360;
    else if (hsv.h >= 360)
        hsv.h = hsv.h % 360;
    this->hsv(hsv.h,(uint8_t)hsv.s,(uint8_t)hsv.v);    
}
RGBLed::addSaturation(int amount) {
    HSV hsv = this->hsv();
    hsv.s += amount;
    if (hsv.s < 0) 
        hsv.s = 0;
    else if (hsv.s > UINT8_MAX)
        hsv.s = UINT8_MAX;
    this->hsv(hsv.h,(uint8_t)hsv.s,(uint8_t)hsv.v);    
}
RGBLed::addBrightness(int amount) {
    HSV hsv = this->hsv();
    hsv.v += amount;
    if (hsv.v < 0) 
        hsv.v = 0;
    else if (hsv.v > UINT8_MAX)
        hsv.v = UINT8_MAX;
    this->hsv(hsv.h,(uint8_t)hsv.s,(uint8_t)hsv.v);    
}
