// Defini

#include <SPI.h> 
#include <LoRa.h>
#include <DHT.h>; //INCLUSÃO DE BIBLIOTECA
#include "Servo.h";
#define DHTTYPE DHT22      // DEFINE O MODELO DO SENSOR (DHT22 / AM2302)
#define DHTPIN 7           // PINO DIGITAL UTILIZADO PELO DHT22
#define trigPin 9          // Define o pino Trigger do HS-04 (Sensor ultrassônico)
#define echoPin 10         // Define o pino Echo do HS-04 (Sensor ultrassônico)   
#define ledRed 13
DHT dht(DHTPIN, DHTTYPE);  // PASSA OS PARÂMETROS PARA A FUNÇÃO

// Define variáveis

long distance;
long duration;

float umi = dht.readHumidity();
float temp = dht.readTemperature();
  
int maximumRangeCm = 200;
int minimumRangeCm = 0;
int posicaoServo;
int posicaoBase;
int posicaoBraco;
int posicaoAntebraco;
int posicaoGarra;

int servoCount = 4;
int servoPins[] = {A1, A2, A3, A4};
String inicioTemperatura = "[TP"; // Tag de temperatura
String inicioUmidade = "[UM";     // Tag de Umidade
String inicioDistancia = "[DT";   // Tag de Distância
String inicioSpeedMotor = "[SM";  // Tag de SpeedMotor
String inicioPeso = "[PS";        // Tag de Peso
String inicioNivel = "[NV";       // Tag de nível
String stringFinal = "]";  
Servo servos[4];

 // Declarando as variáveis
 
unsigned long atual = millis();
unsigned long anterior = 0;
unsigned long resultado = atual - anterior;

int contador = 0;
 // Fim Declarando as variáveis
 
void setup() { 
  Serial.begin(9600); 
  analogReference(INTERNAL); // Altera a tensão máxima nas portas analógicas do Arduino de 5V para 1,1V, para uma melhor precisão do sensor.
  while (!Serial); // Espera a porta serial conectar. É necessário a utilização da porta USB. 
  Serial.println("Emissor LORA");
  dht.begin(); //INICIALIZA A FUNÇÃO
  pinMode(trigPin, OUTPUT); // Seta o pino Trigger do HS-04 como saída
  pinMode(echoPin, INPUT); // Seta o pino Echo do HS-04 como entrada
  delay(2000); //INTERVALO DE 2 SEGUNDO ANTES DE INICIAR

  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  pinMode(A3,OUTPUT);
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  AttachServos();
}

void loop() {

  if(!LoRa.begin(433E6)) 
  { 
  Serial.println("A iniciação da LORA não funcionou!");
  while (1); 
  } 
  Serial.print("Enviando o pacote: "); 
  Serial.println(contador);
  contador++;

  // Envio de pacotes
  // Inicia o envio de pacotes 
  LoRa.beginPacket();

  // O comando LoRa.print envia os pacotes correlatos aos caractéres ou valores lidos no emissor para o receptor.

      /////////////////
     //// MEDIÇÃO ////
    /////////////////

    ////// TEMPERATURA E UMIDADE //////
    SendTemperaturaUmidade(); // Método que envia a temperatura e umidade para o supervisório

    ////// DISTÂNCIA //////
    SendDistancia(); // Método que envia a distância para o supervisório

    ////// PESO //////
    
    ////// NÍVEL //////
    
    ////// SPEED MOTOR //////
     
      /////////////////
     /// CONTROLE ////
    /////////////////
  
    ////// ILUMINAÇÃO //////  
    WriteIluminacao();
      
    ////// BRAÇO ROBÓTICO //////
    WriteArm();
      
    ////// ESTEIRA //////              

    // Fim Inicia o envio de pacotes 
    delay(3000); 

}// Fim Loop

/* FUNÇÕES EXTRAS */

void WriteArm() 
{
  //String recebido = leStringSerial();
  String channel;
  String pos;

  channel = Serial.readStringUntil(':').toInt();
  pos = Serial.readStringUntil('*').toInt();
  // servos[channel].write(pos);
  Serial.println("Ch: " + channel);
  Serial.println("Pos: " + pos);
   
  // channel = recebido.readStringUntil(':');
  // int ch = channel.toInt();
  // pos = recebido.readStringUntil('*');
  // return servos[channel.toInt()].write(pos.toInt());
  //  delay(1000);
 
}


void AttachServos() 
{
  for(int i = 0; i < servoCount; i++) 
  {
    servos[i].attach(servoPins[i]);   
  }
}

long microsecCm(long microsecond) 
{
  return microsecond / 58;
}

void initTrigger() 
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW); 
}

/**
 * Função que lê uma string da Serial
 * e retorna-a
 */
String leStringSerial()
{
  String conteudo = "";
  char caractere;
  
  // Enquanto receber algo pela serial
  while(Serial.available() > 0)
  {
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
     conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }
    Serial.println(conteudo);
   return conteudo;   
}

void WriteIluminacao()
{
     String recebido = leStringSerial();
     if(recebido == "ligaLedRed")
     {
        digitalWrite(ledRed,HIGH);
         delay(1000);
     }
     
     if(recebido == "desligaLedRed")
     {
        digitalWrite(ledRed,LOW);
         delay(1000);
     }
     
/* HABILITAR CASO HAJA AS PORTAS DISPONÍVEIS PARA OS LEDS AMARELO E VERDE
     if(recebido == "ligaLedAmarelo")
     {
        digitalWrite(ledAmarelo,HIGH);
     }
     
     if(recebido == "desligaLedAmarelo")
     {
        digitalWrite(ledAmarelo,LOW);
     }

       if(recebido == "ligaLedVerde")
     {
        digitalWrite(ledVerde,HIGH);
     }
     
     if(recebido == "desligaLedVerde")
     {
        digitalWrite(ledVerde,LOW);
     }

     */     
}


void SendTemperaturaUmidade()
{
    String inicioTemperatura = "[TP"; // Tag de temperatura
    String inicioUmidade = "[UM";    
    String stringFinal = "]";  
     
    // Printa a temperatura e umidade 
    Serial.print(inicioTemperatura+dht.readTemperature()+stringFinal);
    delay(1000);
    Serial.print(inicioUmidade+dht.readHumidity()+stringFinal);
    delay(1000);
}

void SendDistancia()
{
    String inicioDistancia = "[DT";   // Tag de Distância
    String stringFinal = "]"; 
   
    initTrigger();
    duration = pulseIn(echoPin, HIGH);
    distance = microsecCm(duration); 
    Serial.print(inicioDistancia+distance+stringFinal);
    delay(1000);
}
