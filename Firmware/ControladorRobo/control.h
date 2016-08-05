#pragma once

#include <Arduino.h>

// Right model
//float kpesq = (4.86*(500/255)), t1esq = (1.06/3), tetaesq = .02;

// Left model
//float kpdir = (5.15*(500/255)), t1dir = (0.8/3), tetadir = .02;
void controllersSetup();

class Controller
{
private:
    float erro_k1 = 0, u_k1 = 0, ui_k1 = 0;
    float kc, ti, td, ts;
    float taw, eaw_k1 =0;

    const float m_umax = 1.0;
    const float m_umin = -1.0;

public:
    Controller(float kc, float ti, float td, float ts);
    float update(float setpoint, float current_value);
    ~Controller();
};
