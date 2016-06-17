


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
  noInterrupts();           // disable all interrupts

  
  interrupts(); //habilita interrupcoes

 } 