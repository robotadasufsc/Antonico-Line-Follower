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
void eForward(int vel)
{
    digitalWrite(esq_t,LOW);
    digitalWrite(esq_f,HIGH);
    analogWrite(esq,sat(vel));

}

void dForward(int vel)
{
    digitalWrite(dir_f,HIGH);
    digitalWrite(dir_t,LOW);
    analogWrite(dir,sat(vel));
}

void goForward(int vel)
{
    // frente
    digitalWrite(esq_t,LOW);
    digitalWrite(dir_t,LOW);
    digitalWrite(esq_f,HIGH);
    digitalWrite(dir_f,HIGH);

    analogWrite(dir,vel);
    analogWrite(esq,vel);

}
void giraEsquerda()
{
    digitalWrite(dir_t,LOW);
    digitalWrite(dir_f,HIGH);
    digitalWrite(esq_t,HIGH);
    digitalWrite(esq_f,LOW);
    analogWrite(dir,vel_max);
    analogWrite(esq,vel_max);
}
void giraDireita()
{
    digitalWrite(dir_t,HIGH);
    digitalWrite(dir_f,LOW);
    digitalWrite(esq_t,LOW);
    digitalWrite(esq_f,HIGH);

    analogWrite(dir,vel_max);
    analogWrite(esq,vel_max);
}
void turnRight()
{
    digitalWrite(esq_t,LOW);
    digitalWrite(dir_t,LOW);
    digitalWrite(esq_f,HIGH);
    digitalWrite(dir_f,HIGH);

    analogWrite(esq,sat(vel_max));
    analogWrite(dir,sat(vel_max*curva));
}
void turnLeft()
{
    digitalWrite(esq_t,LOW);
    digitalWrite(dir_t,LOW);
    digitalWrite(esq_f,HIGH);
    digitalWrite(dir_f,HIGH);
    analogWrite(esq_f,sat(vel_max*curva));
    analogWrite(dir_f,sat(vel_max));
}

// vira em torno do próprio eixo, se tornou mais promissor em curvas muito fechadas !
void acuteRight()
{
    digitalWrite(dir_t,HIGH);
    digitalWrite(esq_t,LOW);
    digitalWrite(dir_f,LOW);
    digitalWrite(esq_f,HIGH);

    analogWrite(dir,sat(vel_max*curva_fechada));
    analogWrite(esq,sat(vel_max));
}

void acuteLeft()
{
    digitalWrite(dir_t,LOW);
    digitalWrite(esq_t,HIGH);
    digitalWrite(dir_f,HIGH);
    digitalWrite(esq_f,LOW);
    analogWrite(esq,sat(vel_max*curva_fechada));
    analogWrite(dir,sat(vel_max));
}
