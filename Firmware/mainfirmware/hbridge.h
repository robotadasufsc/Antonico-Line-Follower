#pragma once
#include "hal.h"

class HBridge
{
private:
    HBridge& operator = (HBridge& other) = delete;
    HBridge(const HBridge& other) = delete;
    HBridge();

    inline float sat(float x, float min, float max);

public:
    static HBridge& self();
    enum {LEFT, RIGHT};


    void setWheelPWM(uint8_t wheel, float speed);
    void forward(int16_t vel);
};
