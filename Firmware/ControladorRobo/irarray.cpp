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

    memset(m_sensorRaw, 0, sizeof(m_sensorRaw));
    memset(m_sensor, 0, sizeof(m_sensor));
    m_calibrating = false;
    this->loadCalibrationFromEEPROM();
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

// read values from eeprom
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
    memset(m_sensorLow, 0, sizeof(m_sensorLow));
    memset(m_sensorHigh, 255, sizeof(m_sensorHigh));
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
        byte sensorRawByte = (m_sensorRaw[i])>>2;
        m_sensorLow[i] = min(m_sensorLow[i], sensorRawByte);
        m_sensorHigh[i] = max(m_sensorHigh[i], sensorRawByte);
        m_sensor[i] = (uint16_t) map(((float)sensorRawByte), (float)m_sensorLow[i], (float)m_sensorHigh[i], 0.0, 255.0);
    }
}

uint16_t IRArray::sensor(uint8_t i, uint8_t typ = 0)
{
    if( i < sizeof(m_sensor))
    {
        switch(typ)
        {
            case MAX:
                return m_sensorHigh[i];
            break;

            case MIN:
                return m_sensorLow[i];
            break;

            case NORMAL:
            default:
                 return m_sensor[i];
            break;
        }
    }
    else
    {
        return 0;
    }
}

float IRArray::estimateLinePosition()
{

    float sumOfMassXdistance = 0;
    float sumOfMass=0;
    float maxValue = 0;

    for(int i = 0; i < NUMBER_OF_SENSORS; i++)
    {
        sumOfMass += m_sensor[i];
        sumOfMassXdistance += m_sensor[i] *i;
        maxValue = max(m_sensor[i],maxValue);

    }
    if (maxValue > 200) //TODO: Should not be hardcoded
    {
        this->m_lastPosition = sumOfMassXdistance/sumOfMass;
    }
    return this->m_lastPosition;
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
