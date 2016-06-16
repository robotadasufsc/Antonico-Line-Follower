// pinos para leitura analógica
const int sensorPin[]={A0,A1,A2,A3,A4,A5,A6,A7,A8};
// eeprom address for the sensors
int addr=0;

// le os sensores e salva o valor lido de cada um no byte[9]
void readSensors(byte* sVec)
{
  for(int i=0;i<9;i++)
      sVec[i] = analogRead(sensorPin[i]);
}

// escreve os valores de sVec na eeprom para os endereços do branco
void writeCalibWhite(byte* sVec)
{
  for(int i=0;i<9;i++)
    EEPROM.write(addr+i, sVec[i]);
}

// escreve os valores de sVec na eeprom para os endereços do preto
void writeCalibBlack(byte* sVec)
{
  for(int i=0;i<9;i++)
    EEPROM.write(addr+i+9, sVec[i]);
}

// le os valores do branco da eeprom
void readCalibWhite(byte* sVec)
{
  for(int i=0;i<9;i++)
    sVec[i]=EEPROM.read(addr+i);
}

// le os valores de preto da eeprom
void readCalibBlack(byte* sVec)
{
  for(int i=0;i<9;i++)
    sVec[i]=EEPROM.read(addr+i+9);
}
