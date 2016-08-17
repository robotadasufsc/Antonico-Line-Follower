#include <Arduino.h>
#include <math.h>
#include "hal.h"
#include "irarray.h"
#include "hbridge.h"
#include "control.h"
#include "lib/Encoder/Encoder.h"
#include "lib/Exposer/exposer.h"


#define MAINFIRMWARE_DEBUG

#define KC_DIR 1.0
#define TI_DIR 0.2
#define TD_DIR 0

#define KC_ESQ 1.0
#define TI_ESQ 0.2
#define TD_ESQ 0

#define TS 0.02

// If MAINFIRMWARE_DEBUG define print debug message
#ifdef MAINFIRMWARE_DEBUG

char debug_buffer[128];
// [Class]: Function:Line: %s
#define debug(fmt, args ...)  do {sprintf(debug_buffer, "[mainfirmware]: %s:%d: " fmt "\n", __FUNCTION__, __LINE__, ## args); Serial.print(debug_buffer);} while(0)
#else
#define debug(fmt, args ...)
#endif

// constantes
#define FREQ 50.0
#define FREQ_ACT 20

// variables for velocity control
float refDir = 0.0;
float refEsq = 0.0;
float basespeed = 0.3;

// angular position of each wheel, with loss of resolution after some time
// will be used to know robot position
float leftAngPos = 0;
float rightAngPos = 0;

// Encoders setup
Encoder left(LEFT_ENCODER_A, LEFT_ENCODER_B);
Encoder right(RIGHT_ENCODER_A,RIGHT_ENCODER_B);

// PID controllers setup
Controller controle_esq = Controller(KC_ESQ, TI_ESQ, TD_ESQ, TS);
Controller controle_dir = Controller(KC_DIR, TI_DIR, TD_DIR, TS);
Controller directionController = Controller(0.1, 0, 0, 1/FREQ_ACT);

// Communication

Exposer* exposer = &Exposer::self();


void peripheralsSetup()
{
    // Sets switch pins to PULLUP mode
    pinMode(SWITCH_1, INPUT_PULLUP);
    pinMode(SWITCH_2, INPUT_PULLUP);
    Serial.begin(115200);
}

void setup()
{
    long unsigned int startTime = millis();
    peripheralsSetup();

    HBridge::self(); // makes sure that the H-ridge pins are initialized during setup phase.

    controllersSetup();

    if (digitalRead(SWITCH_2))
    {
        IRArray* infrared = &IRArray::self();
        long unsigned int calibrationTimer = millis()+2000; //TODO: Should use encoder data to measure a 360º turn.
        infrared->startCalibration();
        refDir = 1;
        refEsq = -1;
        while (millis()<calibrationTimer)
        {
            infrared->startCalibration();
            infrared->readSensors();
        }
        infrared->endCalibration();
    }
    refDir = 0;
    refEsq = 0;
    delay(100);

    debug("Took %d millis to finish.", millis()-startTime);

    //Expose basespeed
    exposer->registerVariable("basespeed", Exposer::_float, &basespeed);
}

void loop()
{
    unsigned long nextCycle = millis() + 1000/FREQ;
    IRArray* infrared = &IRArray::self();
    while (millis() < nextCycle)
    {
    }
    infrared->readSensors();
    float currentLinePos = infrared->estimateLinePosition();
    float u_direction = directionController.update(4.3, currentLinePos);
    refDir = basespeed + u_direction;
    refEsq = basespeed - u_direction;
    basespeed *= 1.001;

    while (Serial.available())
    {
        uint8_t data = Serial.read();
        exposer->processByte(data);
    }
}

// TIMER1 interruption, sample time for velocity control
ISR(TIMER1_COMPA_vect)
{
    // encoder ticks to angular velocity
    float rightAngularDelta = ((float)right.read()/ENCODER_RESOLUTION)*M_PI;
    float leftAngularDelta = ((float)left.read()/ENCODER_RESOLUTION)*M_PI;

    float rightAngularSpeed = rightAngularDelta*FREQ;
    float leftAngularSpeed = leftAngularDelta*FREQ;

    rightAngPos += rightAngularDelta;
    leftAngPos += leftAngularDelta;

    left.write(0);
    right.write(0);

    float u_esq = controle_esq.update(refEsq, leftAngularSpeed);
    float u_dir = controle_dir.update(refDir, rightAngularSpeed);

    HBridge* bridge = &HBridge::self();
    bridge->setWheelPWM(bridge->LEFT,u_esq);
    bridge->setWheelPWM(bridge->RIGHT,u_dir);

}
