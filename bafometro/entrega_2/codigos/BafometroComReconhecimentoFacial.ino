#define qtdLED 5 // Declaração da quantidade de LEDs da aplicação
int LIMIAR = 100; //Declaração do valor de Limiar
int LEDs[qtdLED]; // Vetor que guardará em cada posição o pino de saída para cada LED 
int sensorPin = 0; // Variável para guardar o valor lido pelo sensor
int tempo; // Variável para o tempo de leitura do sensor 
int lidomaximo; // Variável para armazenar o maior valor lido durante a leitura
int lidominimo; // Variável para armazenar o menor valor lido durante a leitura
int i; // Variável de contagem
int j; // Variável de contagem
int sensorAnterior = 50; // Valor anterior do sensor para detectar sopro
int delta = 20; // Limiar de variação para identificar o sopro
int valor_sensor = 0; // Valor do sensor a ser guardado na detecção do sopro
char flag; // Variável de detecção de rosto recebida pelo python

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial para coleta de dados
  i = 0;
  j = 3;

  while(i < qtdLED) { // Enquanto i for menor que a quantidade de LEDs que foi definida, guarda 
    LEDs[i] = j;      // no vetor um valor (começando de 3 e incrementando) equivalente a um pino digital
    i++;
    j++;
  }

  for(i = 0; i < qtdLED; i++) { // Define os pinos dos LEDs, isto é, neste caso do 2 ao 7, como saída
    pinMode(LEDs[i], OUTPUT); 
  }
  pinMode(13, OUTPUT); // Define o pino 13 como saída para o LED indicador de teste
  pinMode(2, OUTPUT); //Define o pino 2 como saída, representando o bloqueador de partida do carro
}


void loop() {
  PORTB = PORTB ^ 100000; // Inverte o estado do pino digital 13 para dar o efeito de Blink (Piscagem)
  delay(100); // Delay para o efeito de blink do LED, indicando que o teste pode ser feito pelo usuário
  
  digitalWrite(2, HIGH); //Começa bloqueado, até que haja prova que o motorista esteja apto para dirigir

  int sensor = analogRead(sensorPin); // Lê o sensor e guarda na variável instantânea
  
  //Serial.print("Valor instantâneo lido pelo sensor: "); //######### Debug IMORTANTE PARA CALIBRAR O SENSOR 
  //Serial.println(sensor); // Envia o valor lido para a serial //######### Debug IMORTANTE PARA CALIBRAR O SENSOR

  // Enviar a flag
  Serial.println("FLAG:0"); // Não está detectando sopros

  // Detecta sopro LEMBRAR DA FLAG AQUI
  if(abs(sensor - sensorAnterior) > delta) { // Se houver uma variação brusca na leitura, então tem alguém assoprando no sensor
    //Serial.println("Sopro detectado!"); //######### Debug

    // Enviar a flag
    Serial.println("FLAG:1"); // Confirmando para o reconhecimento que está detectando sopros da pessoa que está sendo reconhecida 

    digitalWrite(13, HIGH); // Acende o LED Verde (indicando que o sensor detectou alguém assoprando)
    lidomaximo = 0; // Iniciar o valor máximo lido pelo sensor como 0
    lidominimo = 1023; //Iniciar o valor mínimo lido pelo sensor com o máximo valor possível 

    // Guarda o valor máximo e mínimo lido
    for(tempo = 0; tempo <= 5000; tempo++) { // Faz a leitura do sensor por 5 segundos e atualiza o maior valor lido
      sensor = analogRead(sensorPin);
      delay(1);
      if(sensor > lidomaximo) {
        lidomaximo = sensor; // Armazena o valor máximo lido
      }
      if (sensor < lidominimo) {
        lidominimo = sensor; //Armazena o valor mínimo lido
      }
    }

    if(lidomaximo >= LIMIAR) { // Se a leitura for maior que o valor de limiar, então a pessoa ingeriu álcool
      // Conversão de ADC para concentração em mg/L

      //######### Debug
      //Serial.print("Lidomaximo: ");
      //Serial.println(lidomaximo);

      float alcool_mg_L = (((float(lidomaximo) - float(LIMIAR)) / (1023 - LIMIAR)) * (4 - 0.04)) + 0.04;

      //Serial.print("Concentração de alcool: "); //######### Debug
      Serial.println("ALC:" + String(alcool_mg_L)); // Envia esse valor para o serial (código em python recebe e bloqueia o usuário)

      int nivel = map(lidomaximo, 0, 1023, 0, qtdLED); // Converte a escala do sensor para 0 a 6 (LEDs)
    
      for(i = 0; i < qtdLED; i++) { // Compara todos os pinos dos LEDs com o valor convertido da escala
        if(i < nivel) { // Compara o pino atual da contagem, e se for menor que o nível máximo convertido
          digitalWrite(LEDs[i], HIGH); // Acende o LED
        }
        else { // Compara o pino atual da contagem, e se for maior que o nível máximo convertido
          digitalWrite(LEDs[i], LOW); // Apaga o LED
        }
      }
    
      delay(10000); // Aguarda 10 segundos para o usuário conseguir fazer a leitura do bargraph

      for(i = 0; i < qtdLED; i++) { // Apaga todos os LEDs
        digitalWrite(LEDs[i], LOW);
      }
    }
    else if(lidomaximo <= LIMIAR) {
      float alcool_mg_L = 0.04;

      //Serial.print("Lidominimo: "); //######### Debug
      //Serial.println(lidominimo); //######### Debug

      //Serial.print("Concentração de alcool: "); //######### Debug
      Serial.println("ALC:" + String(alcool_mg_L)); // Envia esse valor para o serial (código em python recebe e libera o usuário)

      digitalWrite(2, LOW); // Libera a partida do veículo
      delay(5000);

    }
  }  
}
