// pinos para leitura analógica
const int sensorPin[]= {A0,A1,A2,A3,A8,A4,A5,A6,A7};
// eeprom address for the sensors
int addr=0;
byte sensorLow[9] = {0};
byte sensorHigh[9] = {1024};
int sensorRaw[9] = {0};
int sensor[9] = {0};


// le os sensores e salva o valor lido de cada um no byte[9]
void readSensors()
{
    for (int i=0; i<9; i++)
        sensorRaw[i] = analogRead(sensorPin[i]);
    sensor[i] = map(sensorRaw[i], sensorLow[i], sensorHigh[i], 0, 255);
}

// escreve os valores de sVec na eeprom para os endereços do branco
void writeCalibWhite(byte* sVec)
{
    for (int i=0; i<9; i++)
        EEPROM.write(addr+i, sVec[i]);
}

// escreve os valores de sVec na eeprom para os endereços do preto
void writeCalibBlack(byte* sVec)
{
    for (int i=0; i<9; i++)
        EEPROM.write(addr+i+9, sVec[i]);
}

// le os valores do branco da eeprom
void readCalibWhite(byte* sVec)
{
    for (int i=0; i<9; i++)
        sVec[i]=EEPROM.read(addr+i);
}

// le os valores de preto da eeprom
void readCalibBlack(byte* sVec)
{
    for (int i=0; i<9; i++)
        sVec[i]=EEPROM.read(addr+i+9);
}
