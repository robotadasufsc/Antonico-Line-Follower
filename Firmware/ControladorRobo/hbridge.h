#include "hal.h"

#define sat(x) (max(min(x,255),-255))


class HBridge
{
private:
    HBridge& operator = (HBridge& other) = delete;
    HBridge(const HBridge& other) = delete;
    HBridge();


public:
    static HBridge& self();
    enum {LEFT_WHEEL, RIGHT_WHEEL};


    void setWheelPWM(uint8_t wheel, int16_t speed);
    void forward(int16_t vel);
};



