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

// posicao angular de cada roda, com provavel perda de resolucao depois de algum tempo.
float posEsq = 0;
float posDir = 0;

long calibrationTimer = 0;
//----------------------------------


// funcoes
void peripheralsSetup(){
    // Seta os pinos de chave como entrada em pull-up
    pinMode(CHAVE1, INPUT_PULLUP);
    pinMode(CHAVE2, INPUT_PULLUP);
    Serial.begin(115200); 
    // aciona os leds infravermelho dos sensores
    digitalWrite(4,HIGH);  
}

void setup()
{
    // initialize serial communications at 9600 bps:

    hBridgeSetup();

    encoderSetup();

    controllersSetup();
     
    if(digitalRead(CHAVE1))
    {   
        calibrationTimer = millis()+4000; //CALCULAR OU MEDIR O TEMPO DE UMA VOLTA
        startCalibration();
        while(millis()<calibrationTimer)
        {
            readSensors();
        }
        endCalibration();
    }


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

    float arcRight = ((float(pulsosDir))/3200.0)*pi; 
    float arcLeft =  ((float(pulsosEsq))/3200.0)*pi; 
    
    wEsq = arcLeft*freq;
    wDir = arcRight*freq;
    
    posDir+=arcRight;
    posEsq+=arcLeft;

    pulsosDir = 0;
    pulsosEsq = 0;

    float u_esq = controle_esq.update(refEsq, wEsq);
    float u_dir = controle_dir.update(refDir, wDir);

    setRightWheel((int)floor(u_dir*51));
    setLeftWheel((int)floor(u_esq*51));

}
