#define qtdLED 6 // Declaração da quantidade de LEDs da aplicação
int LEDs[qtdLED]; // Vetor que guardará em cada posição o pino de saída para cada LED 
int sensorPin = 0; // Variável para guardar o valor lido pelo sensor
int tempo; // Variável para o tempo de leitura do sensor 
int lidomaximo; // Variável para armazenar o maior valor lido durante a leitura
int i; // Variável de contagem
int j; // Variável de contagem

void setup() {
  Serial.begin(9600); // Inicia a comunicação serial para coleta de dados
  i = 0;
  j = 2;

  while(i < qtdLED) { // Enquanto i for menor que a quantidade de LEDs que foi definida, guarda 
    LEDs[i] = j;      // no vetor um valor (começando de 2 e incrementando) equivalente a um pino digital
    i++;
    j++;
  }

  for(i = 0; i < qtdLED; i++) { // Define os pinos dos LEDs (nesse exemplo de 2 ao 7) como saída
    pinMode(LEDs[i], OUTPUT); 
  }
  pinMode(13, OUTPUT); // Define o pino 13 como saída para o LED indicador de teste
}

void loop() {
  PORTB = PORTB ^ 100000; // Inverte o estado do pino digital 13 para dar o efeito de Blink (Piscagem)
  delay(100); // Delay para o efeito de blink do LED, indicando que o teste pode ser feito pelo usuário
  
  int sensor = analogRead(sensorPin); // Lê o sensor e guarda na variável
  Serial.print("Valor lido pelo sensor: "); 
  Serial.println(sensor); // Envia o valor lido para a serial
  
  if(sensor >= 127) { // Se a leitura for maior que o valor de limiar
    digitalWrite(13, HIGH); // Acende o LED Verde (indicando que o sensor detectou um mínimo de álcool)
    lidomaximo = 0; // Iniciar o valor máximo lido pelo sensor como 0

    for(tempo = 0; tempo <= 5000; tempo++) { // Faz a leitura do sensor por 5 segundos e atualiza o maior valor lido
      sensor = analogRead(sensorPin);
      delay(1);
      if(sensor > lidomaximo) {
        lidomaximo = sensor; // Armazena o valor máximo lido
      }
    }

    Serial.print("Maior valor lido: ");
    Serial.println(lidomaximo); // Envia o valor máximo lido para a serial

    digitalWrite(13, LOW); // Após o término da leitura, apaga o LED Verde
    
    int nivel = map(lidomaximo, 0, 500, 0, qtdLED); // Converte a escala de 0 a 500 do sensor em 0 a 6 (LEDs)
    
    Serial.print("Nível de álcool detectado: ");
    Serial.println(nivel); // Envia o nível detectado para a serial
    
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
}
