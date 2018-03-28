#include <SPI.h>
#include <LoRa.h>

     // Declarando as variáveis
 unsigned long atual = millis();
 unsigned long anterior = 0;
 unsigned long resultado = atual - anterior;

int contador = 0;       // Seta o valor inicial da variável do contador em zero
const int LM35 = 0;     // Declara a posição do sensor na porta analógica A0 e seta em zero;
int ValorLido = 0;      // Seta o valor inicial lido no sensor em zero
float temperatura = 0;  // Seta o valor inicial do produto entre o valor lido pelo sensor e a constante de conversão em zero
 
     // Fim Declarando as variáveis

void setup() {
  Serial.begin(9600);
  while (!Serial);               // Espera a porta serial conectar. É necessário a utilização da porta USB.
  analogReference(INTERNAL);     // Altera a tensão máxima nas portas analógicas do Arduino de 5V para 1,1V, para uma melhor precisão do sensor.
  Serial.println("Emissor LORA");

  if (!LoRa.begin(433E6)) {  // Caso a frequência de transmissão da LORA seja diferente de 433E6 Hz, não iniciará a transmissão de pacotes.
    Serial.println("A iniciação da LORA não funcionou!");
    while (1);
  }
}

void loop() {
 
  Serial.print("Enviando o pacote: ");
  Serial.println(contador);
    contador++;
    
ValorLido = analogRead(LM35);                     // Valor lido diretamente pelo pino A0 do sensor
temperatura = ValorLido * 0.1075268817204301;     // Conversão do valor lido na porta analógica (0 a 1023) para graus celsius

    // Envio de pacotes
    // Inicia o envio de pacotes 
  LoRa.beginPacket(); 

   // O comando LoRa.print envia os pacotes correlatos aos caractéres ou valores lidos no emissor para o receptor.

LoRa.println("Descobrequeaspessoascomquemvocêmaisseimportanavidasãotomadasdevocêmuitodepressa,porissosempredevemosdeixaraspessoasqueamamoscompalavrasamorosas,podeseraúltimavezqueasvejamos.Aprendequeascircunstânciaseosambientesteminfluênciasobrenós,masnóssomosresponsáveispornósmesmos.Começaaaprenderquenãosedevecompararcomosoutros,mascomomelhorquepodeser.");
  LoRa.endPacket();      
        
    // Fim Inicia o envio de pacotes 
    
  delay(3000);
}
