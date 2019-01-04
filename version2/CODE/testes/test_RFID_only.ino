#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 4  //GPIO4 ou D2 no nodeMCU, RST no RFID

//do protocolo SPI
#define SS_PIN  5  //GPIO4 ou D1 no nodeMCU, SDA no RFID  
                   //GPIO12 ou D6 no nodeMCU, MISO no RFID
                   //GPIO13 ou D7 no nodeMCU, MOSI no RFID
                   //GPIO14 ou D5 no nodeMCU, SCK no RFID

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void function_RFID (){
  // Procura por cartao RFID
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Seleciona o cartao RFID
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  //Mostra UID na serial
  String conteudo= "";
  for (byte i = 0; i < mfrc522.uid.size; i++){
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  Serial.println(conteudo);
}
void setup (){
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();          // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
}
void loop (){
  function_RFID();
}
