#include <Arduino.h>
#include <EEPROM.h>
#include "caliblib.h"
#include "hbridge.h"
#include "controle.h"

// pinos de interface
#define ledVerde    22
#define ledAmarelo  23
#define ledVerdeOff()  digitalWrite(ledVerde, HIGH)
#define ledVerdeOn() digitalWrite(ledVerde, LOW)
#define ledVerdeToggle() digitalWrite(ledVerde, !digitalRead(ledVerde))
#define ledAmareloOff()  digitalWrite(ledAmarelo, HIGH)
#define ledAmareloOn() digitalWrite(ledAmarelo, LOW)
#define ledAmareloToggle() digitalWrite(ledAmarelo, !digitalRead(ledAmarelo))
//#define analogRead2(x) (analogRead(x)/4.0)

// constantes
#define freq 50.0
#define pi 3.1415

//#define vel_media 0.4
#define vel_min2 1.2 //velocidade mais agressiva
#define vel_max 2.0
#define pesoErro vel_media/25.0

float vel_min = 0.7;
float vel_media = vel_min;

int auxReduz = 0;

//vetores para leitura da linha
float sVecFloat[9]={0};
byte sVecByte[9]={0};
byte sVecByteBlack[9]={0};
byte sVecByteWhite[9]={0};

//calib values defaults
byte maximos[9]={0};
byte minimos[9]={255,255,255,255,255,255,255,255,255};
byte caliBlack[]={0,0,0,0,0,0,0,0,0};
byte caliWhite[]={182,242,252,237,233,248,168,239,251};

//pesos para implementacao de logica
byte pesos[]={1,8,15,20};
//byte pesos[]={1,11,17,22};

//variavel para logica de retorno do robo a pista quando se perde
float erroPassado = 0.0;
//variavel para criar laço mais lento dentro do loop
int contagem=0;

//variaveis para controle de velocidade
float erro = 0.0;
float Refdir = 0.0;
float Refesq = 0.0;
float wdir = 0.0;
float wesq = 0.0;
int pulsosDir = 0;
int pulsosEsq = 0;

//------Struct de controle----------
controlP controlDIR;
controlP controlESQ;
//----------------------------------

//pinos para chaveamento do controle
int chave1 = 52;
int chave2 = 53;

// funcoes
void leituraFloat(float* sVec);
int leituraBin();//For Arduino Mega: (tested on Arduino Mega 2560)
void calibWhiteFunction();
void calibBlackFunction();
float calculaErro();
float calculaRef(char lado, float error);

void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(9600); 

  // aciona os sensores
  digitalWrite(4,HIGH);  

  // Seta os pinos de chave como entrada em pull-up
  pinMode(chave1, INPUT_PULLUP);
  pinMode(chave2, INPUT_PULLUP);

  if(digitalRead(chave2))
  {
    vel_min =  vel_min2; 
  }
  
  // Seta os pinos utilizados pelo Encoder
  pinMode(18, INPUT_PULLUP);
  pinMode(19, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);  

  // Seta os pinos de led
  pinMode(ledVerde, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  
  // Define os pinos do acionamento da ponte H  como saida
  pinMode(esq_f, OUTPUT);
  pinMode(esq_t, OUTPUT);
  pinMode(esq, OUTPUT);
  pinMode(dir_f, OUTPUT);
  pinMode(dir_t, OUTPUT);
  pinMode(dir, OUTPUT);  
  
  // Prepara as interrupcoes para o funcionamneto dos Encoders
  attachInterrupt(2, incrementaEsquerdo, CHANGE); // Pino 21 - Lado Esquerdo
  attachInterrupt(3, incrementaEsquerdo, CHANGE); // Pino 20 - Lado Esquerdo 
  attachInterrupt(4, incrementaDireito, CHANGE); // Pino 19 - Lado Direito
  attachInterrupt(5, incrementaDireito, CHANGE); // Pino 18 - Lado Direito  
  noInterrupts();           // disable all interrupts
  
  //Interrupcao do TIMER1, aka periodo de amostragem para calculo do controle de velocidade
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;  
  TCCR1A = _BV(COM0A1) | _BV(COM0B1) | _BV(WGM00); 
  TCCR1B = _BV(CS00);
  //interrupt frequency (Hz) = (Arduino clock speed 16,000,000Hz) / (prescaler * (compare match register + 1))
  OCR1A = 311; // compare match register for 10us delay
  TCCR1B |= (1 << WGM12); // CTC mode
  //  TCCR1B |= (1 << CS31); // 8 prescaler 
  TCCR1B |= (1 << CS10);
  TCCR1B |= (0 << CS11);
  TCCR1B |= (1 << CS12);
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt
  
  interrupts(); //habilita interrupcoes
  delay(10);
  
  //inicia com motores desligados
  digitalWrite(esq_t,LOW);         
  digitalWrite(dir_t,LOW);
  digitalWrite(esq_f,LOW);         
  digitalWrite(dir_f,LOW);
  
  //leitura de valores de calibracao salvos na EEPROM
  readCalibWhite(caliWhite); 
  readCalibBlack(caliBlack); 
}

void loop() 
{ 
  
  while('a' != Serial.read());
  ledAmareloOn();
  ledVerdeOn();
  //Faz o robo girar
  Refdir = 0.4;
  Refesq = 0.4;
  calibraTudo();
  //calibWhiteFunction();
  ledAmareloToggle();
  ledVerdeToggle();
  goForward(0);
  Refdir = 0.001;
  Refesq = 0.001;
  
  Serial.println();
  Serial.println("Valores gravados na EPROM: Preto");
  for(int i=0;i<9;i++)
  {
    Serial.print(minimos[i]);
    if(i!=8)
    Serial.print(",");
  }
  Serial.println();
  Serial.println("Valores gravados na EPROM: Branco");
  for(int i=0;i<9;i++)
  {
    Serial.print(maximos[i]);
    if(i!=8)
    Serial.print(",");
  }
  //noInterrupts();
  while(1){
    ledAmareloToggle();
    ledVerdeToggle();
    delay(500);
  }
  
}

///////////////////////////////////////////////////////////funcoes//////////////////////////////////////////////////////////////////////////////

void calibraTudo()
{
  byte sensorByteArray[9]={0};
 
  for(int i=0;i<250;i++)
  {
    readSensors(sensorByteArray);
    for(int i=0;i<9;i++)
    {
      if(sensorByteArray[i] > maximos[i])
        maximos[i] = sensorByteArray[i];
      
      if(sensorByteArray[i]< minimos[i])
        minimos[i] = sensorByteArray[i];        
    }
    delay(30);
  }
  writeCalibWhite(maximos);
  writeCalibBlack(minimos);

}

// Funcao para contagem dos pulsos do encoder esquerdo
void incrementaEsquerdo()
{
  pulsosEsq++;
}
// Funcao para contagem dos pulsos do encoder esquerdo
void incrementaDireito()
{
  pulsosDir++;
}

void leituraFloat(float* sVec)
{
  for(int i=0;i<9;i++)
      sVec[i] = ((float)analogRead2(sensorPin[i]))/caliWhite[i];
}

int leituraBin()
{ //retorna os 8 bits do array de sensores
  int sVec=0;
  float valores[]={10.0,0.8,0.8,0.8,0.8,0.8,0.8,0.8};
  //ate 8 para a linha dos sensores, ate 9 para ler o da frente
  for(int i=0;i<8;i++){
     if(((float)((float)(analogRead2(sensorPin[i]))/(float)(caliWhite[i])))<((float)(caliWhite[i]+caliBlack[i])/(2.0*(caliWhite[i]))))
      sVec+=1<<i;
  }
  return sVec;
}

int leituraBin2()
{ //retorna os 8 bits do array + sensor da frente
  int sVec=0;
  //ate 8 para a linha dos sensores, ate 9 para ler o da frente
  for(int i=0;i<9;i++)
  {
     if(((float)((float)(analogRead2(sensorPin[i]))/(float)(caliWhite[i])))<((float)(caliWhite[i]+caliBlack[i])/(2.0*(caliWhite[i]))))
      sVec+=1<<i;     
  }
  return sVec;
}

// calibra o branco e salva na eeprom
void calibWhiteFunction(){
  byte sensorByteArray[9]={0};
  readSensors(sensorByteArray);
  writeCalibWhite(sensorByteArray);
  Serial.println("Salvo os seguintes valores na eeprom para branco");
  for(int i=0;i<9;i++)
  {
    Serial.print(sensorByteArray[i]);
    if(i!=8)
    Serial.print(",");
  }
  Serial.println();
}

// calibra o preto e salva na eeprom
void calibBlackFunction()
{
  byte sensorByteArray[9]={0};
  readSensors(sensorByteArray);
  writeCalibBlack(sensorByteArray);
  Serial.println("Salvo os seguintes valores na eeprom para preto");
  for(int i=0;i<9;i++)
  {
    Serial.print(sensorByteArray[i]);
    if(i!=8)
    Serial.print(",");
  }
  Serial.println();
}

float calculaErro()
{
  int sensorBin = leituraBin();
  float leituraEsq=0;
  float leituraDir=0;
  for(int i=0;i<4;i++)
  {
    leituraDir+=((float)((bool)(((1<<i) & sensorBin))))*((float)pesos[3-i]);
  }
  for(int i=4;i<8;i++)
  {
    leituraEsq+=((float)((bool)(((1<<i) & sensorBin))))*((float)pesos[i-4]);
  }
  
  return -leituraEsq+leituraDir;
}

float calculaRef(char lado, float error)
{
  if (lado=='d')
  {
    float a = vel_media+error*pesoErro;
    if(a>0)
      return (a);
    else
      return 0;
  }
  else
  if (lado=='e')
  {
    float a = vel_media-error*pesoErro;
    
    if(a>0)
      return (a);
    else
      return 0;
  }
  else return 0.0;
}

// interrupcao para TIMER1, periodo de amostragem para controle de velocidade 
ISR(TIMER1_COMPA_vect)
{
  // Calculo das velocidades dos motores
  wdir = (((float(pulsosDir))/3200.0)*freq*pi);
  wesq = (((float(pulsosEsq))/3200.0)*freq*pi);
  pulsosDir = 0;
  pulsosEsq = 0;

  controlDIR = controlaDir(Refesq, wdir, controlDIR);
  controlESQ = controlaEsq(Refdir, wesq, controlESQ);
}
