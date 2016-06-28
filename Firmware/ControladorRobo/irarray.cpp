#include "irarray.h"
#include <EEPROM.h>

//TODO: this need to be in hal
uint16_t IRArray::m_sensorPin[NUMBER_OF_SENSORS] =
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

IRArray::IRArray()
{
    //TODO: this need to be in hal
    m_enablePin = 4;
    pinMode(m_enablePin, OUTPUT);

    memset(m_sensorLow, 0, sizeof(m_sensorLow));
    memset(m_sensorHigh, 255, sizeof(m_sensorHigh));
    memset(m_sensorRaw, 0, sizeof(m_sensorRaw));
    memset(m_sensor, 0, sizeof(m_sensor));
    m_calibrating = false;
    this->turnOn();
}

IRArray& IRArray::self()
{
    static IRArray self;
    return self;
}

void IRArray::saveCalibrationToEEPROM()
{
    for (uint16_t i = CALLIB_ADDRESS; i<CALLIB_ADDRESS + NUMBER_OF_SENSORS; i++)
    {
        EEPROM.write(i, m_sensorLow[i - CALLIB_ADDRESS]);
        EEPROM.write(i + NUMBER_OF_SENSORS, m_sensorHigh[i - CALLIB_ADDRESS]);
    }
}

// le os valores do branco da eeprom
void IRArray::loadCalibrationFromEEPROM()
{
    for (uint16_t i = CALLIB_ADDRESS; i<CALLIB_ADDRESS + NUMBER_OF_SENSORS; i++)
    {
        m_sensorLow[i - CALLIB_ADDRESS] = EEPROM.read(i);
        m_sensorHigh[i - CALLIB_ADDRESS] = EEPROM.read(i + NUMBER_OF_SENSORS);
    }
}


void IRArray::startCalibration()
{
    m_calibrating = true;
}

void IRArray::endCalibration()
{
    m_calibrating = false;
    saveCalibrationToEEPROM();
}

void IRArray::readSensors()
{
    for (int i = 0; i < NUMBER_OF_SENSORS; i++)
    {
        m_sensorRaw[i] = analogRead(m_sensorPin[i]);
        byte sensorRawByte = m_sensorRaw[i]>>2;
        if (m_calibrating)
        {
            m_sensorLow[i] = min(m_sensorLow[i], sensorRawByte);
            m_sensorHigh[i] = max(m_sensorHigh[i], sensorRawByte);
        }
        m_sensor[i] = map(sensorRawByte, m_sensorLow[i], m_sensorHigh[i], 0, 255);
    }
}

uint16_t* IRArray::sensor(uint8_t i)
{
    if( i < sizeof(m_sensor))
    {
        return &m_sensor[i];
    }
    else
    {
        return nullptr;
    }
}

float IRArray::estimateLinePosition()
{
    float sumOfMassXdistance = 0;
    float sumOfMass=0;
    int size = sizeof(m_sensor)/sizeof(m_sensor[0]);
    for(int i=0;i<size;i++)
    {
      sumOfMass += m_sensor[i];
      sumOfMassXdistance += m_sensor[i] *i;
    }
    return sumOfMassXdistance/sumOfMass;
}

bool IRArray::turnOn()
{
    digitalWrite(m_enablePin, HIGH);
    return digitalRead(m_enablePin);
}

bool IRArray::turnOff()
{
    digitalWrite(m_enablePin, LOW);
    return !digitalRead(m_enablePin);
}
