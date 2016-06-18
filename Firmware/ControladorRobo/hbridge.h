int dir_f = 8;
int dir_t = 7;
int esq_f = 6;
int esq_t = 10;
int esq = 5;
int dir = 9;

#define sat(x) (abs(x) > 200 ? (200) : abs(x))

//algumas variaveis para controle de velocidade
int vel_max = 120;
float curva = 0;
float curva_fechada = 2;
float esquerda=1;
float direita=1.1;


void hBridgeSetup()
{
    // Define os pinos do acionamento da ponte H  como saida
    pinMode(esq_f, OUTPUT);
    pinMode(esq_t, OUTPUT);
    pinMode(esq, OUTPUT);
    pinMode(dir_f, OUTPUT);
    pinMode(dir_t, OUTPUT);
    pinMode(dir, OUTPUT);

    //inicia com motores desligados
    digitalWrite(esq_t,LOW);
    digitalWrite(dir_t,LOW);
    digitalWrite(esq_f,LOW);
    digitalWrite(dir_f,LOW);

}


// frente  esquerda
void setLeftWheel(int vel)
{
    vel = max(min(vel,255),-255);
    if (vel<0)
    {
        digitalWrite(esq_t,HIGH);
        digitalWrite(esq_f,LOW);    
    
    }
    else
    {
        digitalWrite(esq_t,LOW);
        digitalWrite(esq_f,HIGH);    
    }
    analogWrite(esq,sat(vel));
}

void setRightWheel(int vel)
{
    vel = max(min(vel,255),-255);
    if (vel<0)
    {
        digitalWrite(dir_f,LOW);
        digitalWrite(dir_t,HIGH);
    }else
    {
        digitalWrite(dir_f,HIGH);
        digitalWrite(dir_t,LOW);
    }
    analogWrite(dir,sat(vel));
}

void goForward(int vel)
{
    eForward(vel);
    dForward(vel);
}
