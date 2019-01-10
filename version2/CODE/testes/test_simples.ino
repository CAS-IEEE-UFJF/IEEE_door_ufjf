//only test
//faz leitura das tag, acende LEDs, buzzer, botão e move o motor. Não tem firebase ou servidor
#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 4  //GPIO4 ou D2 no nodeMCU, RST no RFID

//do protocolo SPI
#define SS_PIN  5  //GPIO5 ou D1 no nodeMCU, SDA no RFID
                   //GPIO12 ou D6 no nodeMCU, MISO no RFID
                   //GPIO13 ou D7 no nodeMCU, MOSI no RFID
                   //GPIO14 ou D5 no nodeMCU, SCK no RFID
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

#define RED_LED 3 //GPIO3 ou RX se funcionar. Se não funcionar, usar os pinos do led junto com os dos motores (UP_ENG e DOWN_ENG)
#define GRE_LED 15  //GPIO15 ou D8
#define BUZZER 16 //GPIO16 ou D0
const int button = A0;

#define UP_ENG 0;   //GPIO0 ou D3
#define DOWN_ENG 2; //GPIO2 ou D4

//esse problema de falta de portas é corrigido usando contador(está dentro de itens  adicionais no github)

void efeito (){
  digitalWrite(GRE_LED, HIGH);
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BUZZER, HIGH);

  digitalWrite(UP_ENG, HIGH);
  digitalWrite(DOWN_ENG, LOW);
  delay(800);
  digitalWrite(BUZZER, LOW);

  digitalWrite(UP_ENG, LOW);
  digitalWrite(DOWN_ENG, HIGH);
  delay(500);

  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(UP_ENG, LOW);
  digitalWrite(DOWN_ENG, LOW);
  delay(500);
}

void function_RFID (){
  // Procura por cartao RFID
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;
  // Seleciona o cartao RFID
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  for (byte i = 0; i < mfrc522.uid.size; i++){
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "01 D1 FC 52" || "03 1C A0 F6"){
    efeito();
  }
}

void setup (){
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();          // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");

  pinMode(RED_LED, OUTPUT);
  pinMode(GRE_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(UP_ENG, OUTPUT);
  pinMode(DOWN_ENG, OUTPUT);
}

void loop (){
  function_RFID();
  if(analogRead(button) >= 1000){
    efeito();
    Serial.println("push button");
  }
}
