#ifndef RGBLed_H_
#define RGBLed_H_

#include <Arduino.h>

typedef struct {
    uint8_t r, g, b;
} RGB;
typedef struct {
    int h, s, v;
} HSV;

class RGBLed
{
public:
    RGBLed() {};
    RGBLed(uint8_t rPin, uint8_t gPin, uint8_t bPin, boolean glareHigh);
    ~RGBLed(){}
    static RGBLed createGlareHigh(uint8_t rPin, uint8_t gPin, uint8_t bPin);
    static RGBLed createGlareLow(uint8_t rPin, uint8_t gPin, uint8_t bPin);

    white();
    red();     green(); blue();
    magenta(); cyan();  yellow();

    on() { white(); };
    off();

    rgb(uint8_t r, uint8_t g, uint8_t b );
    r(uint8_t r); g(uint8_t g); b(uint8_t b);
    uint8_t r() { return val.r;}; 
    uint8_t g() { return val.g;}; 
    uint8_t b() { return val.b;};

    hsv(int hue, uint8_t sat, uint8_t va);
    h(int h); s(uint8_t s); v(uint8_t v);
    int h() { return this->hsv().h; }; 
    int s() { return this->hsv().s; }; 
    int v() { return this->hsv().v; }; 
    

    rotateHue(int amount);
    addSaturation(int amount);
    addBrightness(int amount);

private:
    uint8_t rPin, gPin, bPin;
    boolean glareHigh=true;
    RGB val = {0,0,0};

    uint8_t value(uint8_t vv) { return glareHigh ? vv : UINT8_MAX - vv; };
    uint8_t onValue() { return glareHigh ? HIGH : LOW ;};
    uint8_t offValue() { return glareHigh ? LOW : HIGH ;}; 
    HSV hsv();

    r_on();  g_on();  b_on();
    r_off(); g_off(); b_off();

};

#endif