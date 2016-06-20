#include <Arduino.h>
#include <math.h>
#include "hal.h"
#include "irarray.h"
#include "hbridge.h"
#include "controle.h"
#include "encoder.h"

#define CONTROLADOR_ROBO_DEBUG

// If CONTROLADOR_ROBO_DEBUG define print debug message
#ifdef CONTROLADOR_ROBO_DEBUG
char debug_buffer[128];
// [Class]: Function:Line: %s
#define debug(fmt, args ...)  do {sprintf(debug_buffer, "[ControladorRobo]: %s:%d: " fmt "\n", __FUNCTION__, __LINE__, ## args); Serial.print(debug_buffer);} while(0)
#else
#define debug(fmt, args ...)
#endif

// constantes
#define freq 50.0

//variaveis para controle de velocidade
float refDir = 0.0;
float refEsq = 0.0;

// posicao angular de cada roda, com provavel perda de resolucao depois de algum tempo.
float posEsq = 0;
float posDir = 0;
//----------------------------------


// funcoes
void peripheralsSetup()
{
    // Seta os pinos de chave como entrada em pull-up
    pinMode(SWITCH1, INPUT_PULLUP);
    pinMode(SWITCH2, INPUT_PULLUP);
    Serial.begin(115200);
}

void setup()
{
    long unsigned int startTime = millis();
    peripheralsSetup();

    HBridge::self();

    encoderSetup();

    controllersSetup();

    if (digitalRead(SWITCH1))
    {
        IRArray* infrared = &IRArray::self();
        long unsigned int calibrationTimer = millis()+4000; //TODO: medir uma volta completa.
        infrared->startCalibration();
        while (millis()<calibrationTimer)
        {
            infrared->startCalibration();
        }
        infrared->endCalibration();
    }
    refDir = 1.0;
    refEsq = 1.0;
    delay(4000);

    debug("Took %d millis to finish.", millis()-startTime);
}

void loop()
{
    delay(100);
}

///////////////////////////////////////////////////////////funcoes//////////////////////////////////////////////////////////////////////////////


// interrupcao para TIMER1, periodo de amostragem para controle de velocidade
ISR(TIMER1_COMPA_vect)
{
    // Calculo das velocidades dos motores

    float arcRight = ((float(pulsosDir))/3200.0)*M_PI;
    float arcLeft = ((float(pulsosEsq))/3200.0)*M_PI;

    wEsq = arcLeft*freq;
    wDir = arcRight*freq;

    posDir+=arcRight;
    posEsq+=arcLeft;

    pulsosDir = 0;
    pulsosEsq = 0;

    float u_esq = controle_esq.update(refEsq, wEsq);
    float u_dir = controle_dir.update(refDir, wDir);

    HBridge* bridge = &HBridge::self();
    bridge->setWheelPWM(bridge->LEFT,(int)floor(u_dir*51));
    bridge->setWheelPWM(bridge->RIGHT,(int)floor(u_esq*51));

}
