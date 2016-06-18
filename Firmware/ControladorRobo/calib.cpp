#include "calib.h"
#include <EEPROM.h>

// pinos para leitura analógica
uint16_t Calib::m_sensorPin[NUMBER_OF_SENSORS] =
{
    A0,
    A1,
    A2,
    A3,
    A8,
    A4,
    A5,
    A6,
    A7,
};

Calib::Calib()
{
    memset(m_sensorLow, 0, sizeof(m_sensorLow));
    memset(m_sensorHigh, 255, sizeof(m_sensorHigh));
    memset(m_sensorRaw, 0, sizeof(m_sensorRaw));
    memset(m_sensor, 0, sizeof(m_sensor));
    m_calibrating = false;
}

Calib& Calib::self()
{
    static Calib self;
    return self;
}

void Calib::saveCalibToEEPROM()
{
    for (uint16_t i = CALLIB_ADDRESS; i<CALLIB_ADDRESS + NUMBER_OF_SENSORS; i++)
    {
        EEPROM.write(i, m_sensorLow[i - CALLIB_ADDRESS]);
        EEPROM.write(i + NUMBER_OF_SENSORS, m_sensorHigh[i - CALLIB_ADDRESS]);
    }
}

// le os valores do branco da eeprom
void Calib::loadCalibFromEEPROM()
{
    for (uint16_t i = CALLIB_ADDRESS; i<CALLIB_ADDRESS + NUMBER_OF_SENSORS; i++)
    {
        m_sensorLow[i - CALLIB_ADDRESS] = EEPROM.read(i);
        m_sensorHigh[i - CALLIB_ADDRESS] = EEPROM.read(i + NUMBER_OF_SENSORS);
    }
}


void Calib::startCalibration()
{
    m_calibrating = true;
}

void Calib::endCalibration(){
    m_calibrating = false;
    saveCalibToEEPROM();
}

void Calib::readSensors()
{
    for(int i = 0; i < NUMBER_OF_SENSORS; i++)
    {
        m_sensorRaw[i] = analogRead(m_sensorPin[i]);
        byte sensorRawByte = m_sensorRaw[i]>>2;
        if(m_calibrating)
        {
            m_sensorLow[i] = min(m_sensorLow[i], sensorRawByte);
            m_sensorHigh[i] = max(m_sensorHigh[i], sensorRawByte);
        }
        m_sensor[i] = map(sensorRawByte, m_sensorLow[i], m_sensorHigh[i], 0, 255);
    }
}
