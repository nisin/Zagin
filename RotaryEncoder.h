
#include <Arduino.h>

class RotaryEncoder
{
public:
    RotaryEncoder() {};
    // RotaryEncoder's A,B pin number and cog is skip count.   
    RotaryEncoder(uint8_t pinA, uint8_t pinB, uint8_t cog=2);
    ~RotaryEncoder();
    // 回転を取得する。 -1,0,1 
    int read();
    // 状態を一旦フラッシュする
    void flash();

private:
    uint8_t pinA;
    uint8_t pinB;
    int last=0;
    uint8_t cog;
    int8_t cogcnt=0;
};

