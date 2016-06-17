#include <Arduino.h>
#include <EEPROM.h>
#include "caliblib.h"
#include "hbridge.h"
#include "controle.h"
#include "encoder.h"

//DOIDJERA ABSOLUTA
#define analogRead(x) (analogRead(x)/4.0)

// constantes
#define freq 50.0
#define pi 3.1415

// pinos de interface
#define ledVerde        22
#define ledAmarelo      23
#define ledVerdeOff()   digitalWrite(ledVerde, HIGH)
#define ledVerdeOn()    digitalWrite(ledVerde, LOW)
#define ledVerdeToggle() digitalWrite(ledVerde, !digitalRead(ledVerde))
#define ledAmareloOff()  digitalWrite(ledAmarelo, HIGH)
#define ledAmareloOn()   digitalWrite(ledAmarelo, LOW)
#define ledAmareloToggle() digitalWrite(ledAmarelo, !digitalRead(ledAmarelo))

//vetores para leitura da linha
float sVecFloat[9]={0};
byte sVecByte[9]={0};
byte sVecByteBlack[9]={0};
byte sVecByteWhite[9]={0};


//pesos para implementacao de logica
byte pesos[]={1,8,15,20};
//byte pesos[]={1,11,17,22};
//variavel para logica de retorno do robo a pista quando se perde
float erroPassado = 0.0;
//variavel para criar laço mais lento dentro do loop
int contagem=0;
byte breakcount=0;

 int sensores[] = {0, 1, 2, 3, 4, 9, 5 ,6 ,7 ,8};

//variaveis para controle de velocidade
float Refdir = 0.0;
float Refesq = 0.0;


//----------------------------------

//pinos para chaveamento do controle
int chave1 = 52;
int chave2 = 53;

// funcoes
void leituraFloat(float* sVec);
int leituraBin();//For Arduino Mega: (tested on Arduino Mega 2560)
float calculaRef(char lado, float error);

void setup()
{
  // initialize serial communications at 9600 bps:
  Serial.begin(115200); 
  // aciona os sensores
  digitalWrite(4,HIGH);  
  // Seta os pinos de chave como entrada em pull-up
  pinMode(chave1, INPUT_PULLUP);
  pinMode(chave2, INPUT_PULLUP);
  if(digitalRead(chave2)){
  }

  hBridgeSetup();

  encoderSetup();

  controllersSetup();
    
  //leitura de valores de calibracao salvos na EEPROM
  readCalibWhite(caliWhite); 
  readCalibBlack(caliBlack); 


  Refdir = 2.0;
  Refesq = 1.0;
  delay(4000);
}

void loop() 
{ 

  delay(100);  
}

///////////////////////////////////////////////////////////funcoes//////////////////////////////////////////////////////////////////////////////


void leituraFloat(float* sVec){
  for(int i=0;i<9;i++)
      sVec[i] = ((float)analogRead(sensorPin[i]))/caliWhite[i];
}

//retorna os 8 bits do array de sensores
int leituraBin(){ 
 
  //ate 8 para a linha dos sensores, ate 9 para ler o da frente
  for(int sensor: sensores){
      Serial.println(sensor);
  }
  
}

//retorna os 8 bits do array + sensor da frente
int leituraBin2(){
  int sVec=0;
  //ate 8 para a linha dos sensores, ate 9 para ler o da frente
  for(int i=0;i<9;i++){
     if(((float)((float)(analogRead(sensorPin[i]))/(float)(caliWhite[i])))<((float)(caliWhite[i]+caliBlack[i])/(2.0*(caliWhite[i]))))
      sVec+=1<<i;     
  }
  return sVec;
}

// interrupcao para TIMER1, periodo de amostragem para controle de velocidade 
ISR(TIMER1_COMPA_vect){
  // Calculo das velocidades dos motores
  wesq = (((float(pulsosDir))/3200.0)*freq*pi);
  wdir = (((float(pulsosEsq))/3200.0)*freq*pi);
  pulsosDir = 0;
  pulsosEsq = 0;

  float u_esq = controle_esq.update(Refesq, wesq);
  float u_dir = controle_dir.update(Refdir, wdir);

  Serial.println(u_dir);
  dForward((int)floor(u_dir*51));
  eForward((int)floor(u_esq*51));

}
