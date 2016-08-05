#pragma once

#include <Arduino.h>

#define kc_dir 5.0
#define ti_dir 0.2
#define td_dir 0

#define kc_esq 5.0
#define ti_esq 0.2
#define td_esq 0

#define Ts 0.02
#define umin -5.0
#define umax 5.0

//Modelo Esquerdo
//float kpesq = (4.86*(500/255)), t1esq = (1.06/3), tetaesq = .02;

//Modelo Direito
//float kpdir = (5.15*(500/255)), t1dir = (0.8/3), tetadir = .02;
void controllersSetup();

class Controller
{
private:
    float erro_k1 = 0, u_k1 = 0, ui_k1 = 0;
    float kc, ti, td, ts;
    float taw, eaw_k1 =0;

public:
    Controller(float kc, float ti, float td, float ts);
    float update(float setpoint, float current_value);
    ~Controller();
};
