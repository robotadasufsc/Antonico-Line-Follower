// pinos para leitura analógica
const int sensorPin[]= {A0,A1,A2,A3,A8,A4,A5,A6,A7};
// eeprom address for the sensors
#define CALLIBADDRESS 0

byte sensorLow[9] = {0};
byte sensorHigh[9] = {255};
int sensorRaw[9] = {0};
int sensor[9] = {0};

// le os sensores e salva o valor lido de cada um no byte[9]
int calibrating = 0;


void saveCalibToEEPROM()
{
    for (int i=0; i<9; i++)
    {
        EEPROM.write(CALLIBADDRESS+i, sensorLow[i]);
        EEPROM.write(CALLIBADDRESS+i+9, sensorHigh[i]);
    }
}


// le os valores do branco da eeprom
void loadCalibFromEEPROM()
{
    for (int i=0; i<9; i++)
    {
        sensorLow[i]=EEPROM.read(CALLIBADDRESS+i);
        sensorHigh[i]=EEPROM.read(CALLIBADDRESS+i+9);
    }
}


void startCalibration(){
    calibrating = 1;
}


void endCalibration(){
    calibrating = 0;
    saveCalibToEEPROM();
}


void readSensors()
{
    for(int i=0;i<9;i++)
    {
        sensorRaw[i] = analogRead(sensorPin[i])/4.0;
        if(calibrating)
        {
            sensorLow[i] = min(sensorLow[i],sensorRaw[i]); 
            sensorHigh[i] = max(sensorHigh[i],sensorRaw[i]);
        }
        sensor[i] = map(sensorRaw[i], sensorLow[i], sensorHigh[i], 0, 255);
    }
}
