int pulsosDir = 0;
int pulsosEsq = 0;

float wEsq = 0;
float wDir = 0;

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


void encoderSetup()
{

    // Seta os pinos utilizados pelo Encoder
    pinMode(18, INPUT_PULLUP);
    pinMode(19, INPUT_PULLUP);
    pinMode(20, INPUT_PULLUP);
    pinMode(21, INPUT_PULLUP);

    // Prepara as interrupcoes para o funcionamneto dos Encoders
    attachInterrupt(2, incrementaEsquerdo, CHANGE); // Pino 21 - Lado Esquerdo
    attachInterrupt(3, incrementaEsquerdo, CHANGE); // Pino 20 - Lado Esquerdo
    attachInterrupt(4, incrementaDireito, CHANGE); // Pino 19 - Lado Direito
    attachInterrupt(5, incrementaDireito, CHANGE); // Pino 18 - Lado Direito

}