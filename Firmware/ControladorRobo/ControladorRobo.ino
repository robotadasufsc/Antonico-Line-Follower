#include <Arduino.h>
#include <math.h>
#include "hal.h"
#include "calib.h"
#include "hbridge.h"
#include "controle.h"
#include "encoder.h"

// constantes
#define freq 50.0

//variaveis para controle de velocidade
float refDir = 0.0;
float refEsq = 0.0;

// posicao angular de cada roda, com provavel perda de resolucao depois de algum tempo.
float posEsq = 0;
float posDir = 0;

long calibrationTimer = 0;
//----------------------------------


// funcoes
void peripheralsSetup(){
    // Seta os pinos de chave como entrada em pull-up
    pinMode(SWITCH1, INPUT_PULLUP);
    pinMode(SWITCH2, INPUT_PULLUP);
    Serial.begin(115200);
    // aciona os leds infravermelho dos sensores
    digitalWrite(4,HIGH);
}

void setup()
{
    HBridge::self();

    encoderSetup();

    controllersSetup();

    if(digitalRead(SWITCH1))
    {
        Calib* calib = &Calib::self();
        calibrationTimer = millis()+4000; //CALCULAR OU MEDIR O TEMPO DE UMA VOLTA
        calib->startCalibration();
        while(millis()<calibrationTimer)
        {
            calib->startCalibration();
        }
        calib->endCalibration();
    }
    refDir = 1.0;
    refEsq = 1.0;
    delay(4000);
}

void loop()
{
    delay(100);
}

///////////////////////////////////////////////////////////funcoes//////////////////////////////////////////////////////////////////////////////


// interrupcao para TIMER1, periodo de amostragem para controle de velocidade
ISR(TIMER1_COMPA_vect){
    // Calculo das velocidades dos motores

    float arcRight = ((float(pulsosDir))/3200.0)*M_PI;
    float arcLeft =  ((float(pulsosEsq))/3200.0)*M_PI;

    wEsq = arcLeft*freq;
    wDir = arcRight*freq;

    posDir+=arcRight;
    posEsq+=arcLeft;

    pulsosDir = 0;
    pulsosEsq = 0;

    float u_esq = controle_esq.update(refEsq, wEsq);
    float u_dir = controle_dir.update(refDir, wDir);
    
    HBridge* bridge = &HBridge::self();
    bridge->setWheelPWM(bridge->LEFT_WHEEL,(int)floor(u_dir*51));
    bridge->setWheelPWM(bridge->RIGHT_WHEEL,(int)floor(u_esq*51));

}
