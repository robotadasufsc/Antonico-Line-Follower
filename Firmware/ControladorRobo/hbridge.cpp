#include "hbridge.h"

HBridge::HBridge()
{
    pinMode(LEFT_WHEEL_FORWARD_PIN, OUTPUT);
    pinMode(LEFT_WHEEL_BACKWARD_PIN, OUTPUT);
    pinMode(LEFT_WHEEL_SPEED_PIN, OUTPUT);

    pinMode(RIGHT_WHEEL_FORWARD_PIN, OUTPUT);
    pinMode(RIGHT_WHEEL_BACKWARD_PIN, OUTPUT);
    pinMode(RIGHT_WHEEL_SPEED_PIN, OUTPUT);

    //inicia com motores desligados
    digitalWrite(LEFT_WHEEL_FORWARD_PIN,LOW);
    digitalWrite(LEFT_WHEEL_BACKWARD_PIN,LOW);
    digitalWrite(RIGHT_WHEEL_FORWARD_PIN,LOW);
    digitalWrite(RIGHT_WHEEL_BACKWARD_PIN,LOW);

}

HBridge& HBridge::self()
{
    static HBridge self;
    return self;
}

inline float HBridge::sat(float x, float min, float max)
{
    return max(min(x, min), max);
}

void HBridge::setWheelPWM(uint8_t wheel, float speed)
{
    speed = sat(speed,1,-1);
    uint8_t PWM = (uint8_t)(float)(abs(speed)*255.0);
    uint8_t pin_f; //forward pin
    uint8_t pin_b; //backward pin
    uint8_t pin_s; //signal pin
    switch (wheel)
    {
    case LEFT:
        pin_f = LEFT_WHEEL_FORWARD_PIN;
        pin_b = LEFT_WHEEL_BACKWARD_PIN;
        pin_s = LEFT_WHEEL_SPEED_PIN;
        break;
    case RIGHT:
        pin_f = RIGHT_WHEEL_FORWARD_PIN;
        pin_b = RIGHT_WHEEL_BACKWARD_PIN;
        pin_s = RIGHT_WHEEL_SPEED_PIN;
        break;
    }

    if (speed>0)
    {
        digitalWrite(pin_f,HIGH);
        digitalWrite(pin_b,LOW);
    }
    else if(speed == 0)
    {
        digitalWrite(pin_f,HIGH);
        digitalWrite(pin_b,HIGH);
    }
    else
    {
        digitalWrite(pin_f,LOW);
        digitalWrite(pin_b,HIGH);
    }
    analogWrite(pin_s, PWM);
}

void HBridge::forward(int16_t vel)
{
    setWheelPWM(LEFT, vel);
    setWheelPWM(RIGHT, vel);
}
