#include <Arduino.h>
#include <math.h>
#include "hal.h"
#include "irarray.h"
#include "hbridge.h"
#include "controle.h"
#include "lib/Encoder/Encoder.h"

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
// será usada para controle de posicao do robô (girar 90º, por exemplo)
float leftAngPos = 0;
float rightAngPos = 0;

// Encoders setup
Encoder left(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder right(RIGHT_ENCODER_A,RIGHT_ENCODER_B);

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

    controllersSetup();

    if (digitalRead(SWITCH2))
    {
        IRArray* infrared = &IRArray::self();
        long unsigned int calibrationTimer = millis()+4000; //TODO: medir uma volta completa.
        infrared->startCalibration();
        refDir = 1;
        refEsq = -1;
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

    float rightAngularDelta = ((float)right.read()/ENCODER_RESOLUTION)*M_PI;
    float leftAngularDelta = ((float)left.read()/ENCODER_RESOLUTION)*M_PI;

    float rightAngularSpeed = rightAngularDelta*freq;
    float leftAngularSpeed = leftAngularDelta*freq;

    rightAngPos += rightAngularDelta;
    leftAngPos += leftAngularDelta;

    left.write(0);
    right.write(0);

    float u_esq = controle_esq.update(refEsq, leftAngularSpeed);
    float u_dir = controle_dir.update(refDir, rightAngularSpeed);

    HBridge* bridge = &HBridge::self();
    bridge->setWheelPWM(bridge->LEFT,(int)floor(u_esq*51));
    bridge->setWheelPWM(bridge->RIGHT,(int)floor(u_dir*51));

}
