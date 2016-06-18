#include <Arduino.h>

// Passar parte da eeprom para um outra classe
// eeprom address for the sensors
#define CALLIB_ADDRESS 0

#define NUMBER_OF_SENSORS 9

class Calib
{
private:
    Calib& operator = (Calib& other) = delete;
    Calib(const Calib& other) = delete;
    Calib();

    void saveCalibToEEPROM();
    void loadCalibFromEEPROM();

    // pinos para leitura analógica
    static uint16_t m_sensorPin[NUMBER_OF_SENSORS];
    byte m_sensorLow[NUMBER_OF_SENSORS];
    byte m_sensorHigh[NUMBER_OF_SENSORS];
    uint16_t m_sensorRaw[NUMBER_OF_SENSORS];
    uint16_t m_sensor[NUMBER_OF_SENSORS];

    // le os sensores e salva o valor lido de cada um no byte[9]
    bool m_calibrating;

public:
    static Calib& self();

    void startCalibration();
    void endCalibration();
    void readSensors();
};
