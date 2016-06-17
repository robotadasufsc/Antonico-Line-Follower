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
#define CHAVE1 52
#define CHAVE2 53


#define ledVerdeOff()      digitalWrite(ledVerde, HIGH)
#define ledVerdeOn()       digitalWrite(ledVerde, LOW)
#define ledVerdeToggle()   digitalWrite(ledVerde, !digitalRead(ledVerde))
#define ledAmareloOff()    digitalWrite(ledAmarelo, HIGH)
#define ledAmareloOn()     digitalWrite(ledAmarelo, LOW)
#define ledAmareloToggle() digitalWrite(ledAmarelo, !digitalRead(ledAmarelo))



//variaveis para controle de velocidade
float refDir = 0.0;
float refEsq = 0.0;

//----------------------------------


// funcoes

void setup()
{
  pinMode(CHAVE1, INPUT_PULLUP);
  pinMode(CHAVE2, INPUT_PULLUP);

  // initialize serial communications at 9600 bps:
  Serial.begin(115200); 
  // aciona os sensores
  digitalWrite(4,HIGH);  
  // Seta os pinos de chave como entrada em pull-up

  hBridgeSetup();

  encoderSetup();

  controllersSetup();
   

  refDir = 1.0;
  refEsq = 1.0;
  delay(4000);
}

void loop() 
{ 

  delay(100);  
}

///////////////////////////////////////////////////////////funcoes//////////////////////////////////////////////////////////////////////////////


// interrupcao para TIMER1, periodo de amostragem para controle de velocidade 
ISR(TIMER1_COMPA_vect){
  // Calculo das velocidades dos motores
  wesq = (((float(pulsosDir))/3200.0)*freq*pi);
  wdir = (((float(pulsosEsq))/3200.0)*freq*pi);
  pulsosDir = 0;
  pulsosEsq = 0;

  float u_esq = controle_esq.update(refEsq, wesq);
  float u_dir = controle_dir.update(refDir, wdir);

  dForward((int)floor(u_dir*51));
  eForward((int)floor(u_esq*51));

}
