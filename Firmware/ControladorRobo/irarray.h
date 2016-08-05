#pragma once

#include <Arduino.h>

// Passar parte da eeprom para um outra classe
// eeprom address for the sensors
#define CALLIB_ADDRESS 0

#define NUMBER_OF_SENSORS 9

#define map(x, in_min, in_max, out_min, out_max) (((float)x - (float)in_min) * ((float)out_max - (float)out_min) / ((float)in_max - (float)in_min) + (float)out_min)

class IRArray
{
private:
    IRArray& operator = (IRArray& other) = delete;
    IRArray(const IRArray& other) = delete;
    IRArray();
    float m_lastPosition;

    void saveCalibrationToEEPROM();
    void loadCalibrationFromEEPROM();

    // pinos para leitura analógica
    static uint16_t m_sensorPin[NUMBER_OF_SENSORS];
    uint8_t m_sensorLow[NUMBER_OF_SENSORS];
    uint8_t m_sensorHigh[NUMBER_OF_SENSORS];
    byte m_enablePin;
    uint16_t m_sensorRaw[NUMBER_OF_SENSORS];
    uint16_t m_sensor[NUMBER_OF_SENSORS];

    // le os sensores e salva o valor lido de cada um no byte[9]
    bool m_calibrating;

public:
    static IRArray& self();

    enum {NORMAL, MAX, MIN};
    void startCalibration();
    void endCalibration();
    void readSensors();
    bool turnOn();
    bool turnOff();
    float estimateLinePosition();
    uint16_t sensor(uint8_t i, uint8_t typ);
};
