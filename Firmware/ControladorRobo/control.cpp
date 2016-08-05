#include "control.h"

void controllersSetup()
{
    //Interrupcao do TIMER1, aka periodo de amostragem para calculo do controle de velocidade
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    TCCR1A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00);
    TCCR1B = _BV(CS00);
    //interrupt frequency (Hz) = (Arduino clock speed 16,000,000Hz) / (prescaler * (compare match register + 1))
    OCR1A = 311; // compare match register for 10us delay
    TCCR1B |= (1 << WGM12); // CTC mode
    //  TCCR1B |= (1 << CS31); // 8 prescaler
    TCCR1B |= (1 << CS10);
    TCCR1B |= (0 << CS11);
    TCCR1B |= (1 << CS12);
    TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
}


Controller::Controller(float kc, float ti, float td, float ts)
{
    this->kc = kc;
    this->ti = ti;
    this->td = td;
    this->ts = ts;
    this->taw = ti;
}

Controller::~Controller()
{
}

float Controller::update(float setpoint, float current_value)
/* Returns control output */
{
    float erro,up,ui,ud, u_total, u_sat;
    erro = setpoint - current_value;
    up = kc*erro;                           //termo proporcional
    ui = ui_k1 + ((kc*ts)/ti)*erro_k1;      //termo integral
    ud = ((kc*td)/ts)*(erro - erro_k1);     //termo derivativo
    ui= ui + (taw)*eaw_k1;               //termo anti windup

    u_total = up + ui + ud;

    u_sat = min(umax,max(u_total,umin)); //saturação

    eaw_k1 = u_sat - u_total; // erro de saturação

    erro_k1 = erro;
    ui_k1 = ui;

    return u_sat;
}
