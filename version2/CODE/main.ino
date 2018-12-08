/*******************************/
/*  atualizacao                */
/*  29/10/2018                 */
/*******************************/
//library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

#include <Arduino.h> 
#include <SPI.h> //protocol
#include <MFRC522.h> //RFID
/************************/

/************************/
#define RST_PIN 5
#define SS_PIN 15
MFRC522 mfrc522(SS_PIN, RST_PIN); 

#define RED_LED D4
#define GRE_LED D3
#define BUZZER  D2

const int button = A0;
/************************/

/************************/
//const char* ssid = "";
//const char* password = "";

const char* ssid = "";
const char* password = "";

#define FIREBASE_HOST ""
#define FIREBASE_AUTH ""

ESP8266WebServer server(80);
/************************/

/************************/
String tag_now = "";
int tag_db = 0;

int tam = 52; //tam db
String database [] = {
};


String tag_backup[32] = ""; //codigo da tag
int tag_db_backup[32]; //posicao da tag
int tag_y_db_backup[32]; //esta no db?
/************************/

/************************/
void handleRoot (){
  String textHTML;

  textHTML += "<!DOCTYPE HTML>";
  textHTML += "<html>";
    textHTML += "<head>";
      textHTML += " <meta http-equiv='refresh' content='3'>";
      textHTML += "<title>ESP8266 Server</title>";
      textHTML += "<style>";
        textHTML += "body{background-color: rgb(235, 235, 245, 100);}";
      textHTML += "</style>";
    textHTML += "</head>";
    
    textHTML += "<body>";
      textHTML += "<center>";
      textHTML += "<h1>TAG</h1>";

      /************************/
      for(int i = 0; i < 32; i++){
        textHTML += tag_backup[i];
        textHTML += ", ";

        if(tag_y_db_backup[i] == 1){
        textHTML += database[tag_db_backup[i] + 1];
        textHTML += "<br>";
        }
        else{
          textHTML += "unknown<br>";
        }
      }
      /************************/

    textHTML += "</body>";
  textHTML += "</html>";
   
  server.send(200, "text/html", textHTML);
}

void handleNotFound (){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
/************************/

void go_in (){ //normal go_in
  digitalWrite(GRE_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  tone(BUZZER, 294);
  delay(100);
  noTone(BUZZER);

  tone(BUZZER, 349);
  delay(110);
  noTone(BUZZER);

  tone(BUZZER, 440);
  delay(110);
  noTone(BUZZER);

  tone(BUZZER, 294);
  delay(120);
  noTone(BUZZER);

  tone(BUZZER, 523);
  delay(180);
  noTone(BUZZER);

  delay(500); //delay here
  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, LOW);

  delay(50);
  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  delay(100); 
  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void go_in_DEV (){ //for developer
  digitalWrite(GRE_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  tone(BUZZER, 294);
  delay(100);
  noTone(BUZZER);

  tone(BUZZER, 349);
  delay(110);
  noTone(BUZZER);

  tone(BUZZER, 440);
  delay(110);
  noTone(BUZZER);

  tone(BUZZER, 294);
  delay(120);
  noTone(BUZZER);

  tone(BUZZER, 523);
  delay(180);
  noTone(BUZZER);

  delay(100);
  tone(BUZZER, 523);
  delay(180);
  noTone(BUZZER);

  delay(500); //delay here
  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, LOW);

  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  delay(100); 
  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void donot_go_in (){ //tag nao cadastrada
  digitalWrite(RED_LED, HIGH);

  tone(BUZZER, 523);
  delay(180);
  noTone(BUZZER);

  tone(BUZZER, 294);
  delay(120);
  noTone(BUZZER);

  tone(BUZZER, 440);
  delay(110);
  noTone(BUZZER);

  tone(BUZZER, 349);
  delay(110);
  noTone(BUZZER);

  tone(BUZZER, 294);
  delay(100);
  noTone(BUZZER);

  digitalWrite(RED_LED, LOW);
}

void up_firebase (String tag, boolean go_db){
  if(go_db)
    Firebase.pushString("tag_read", tag + ", " + database[tag_db_backup[0] + 1]);
  else
    Firebase.pushString("tag_read", tag + ", " + "unknown");

  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
  }

  Serial.print("firebase: tag now: ");
  Serial.println(tag);
}

void function_RFID (){ 
  /*essa funcao faz leitura do EFID e verifica 
    se a tag esta no banco de dados. se sim, chama
    a funcao go_in ou go_in_DEV se for tag especial(developer)
  */

  // Procura por cartao RFID
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return;

  // Seleciona o cartao RFID
  if ( ! mfrc522.PICC_ReadCardSerial())
    return;

  //Mostra UID na serial
  //Serial.print("UID da tag :");
  String conteudo= "";
  for (byte i = 0; i < mfrc522.uid.size; i++){
     //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     //Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //Serial.println();
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "01 D1 FC 52" || "03 1C A0 F6"){ //??
    for(int i = 31; i > 0; i--){
      tag_backup [i] = tag_backup [i - 1];
      tag_db_backup [i] = tag_db_backup [i - 1];
      tag_y_db_backup [i] = tag_y_db_backup [i - 1];
    }

    boolean go = false; //var aux
    tag_now = conteudo.substring(1);

    tag_backup[0] = tag_now;

    for(int i = 0; i < tam; i++){
      if (database[i] == tag_now){
        Serial.println("Yes db");
  
        if ((database[i + 1] == "wesley") ||
            (database[i + 1] == "Walmor") ||
            (database[i + 1] == "Dalila"))
          go_in_DEV();

        else
          go_in();

        tag_db_backup[0] = i;

        go = true;

        tag_y_db_backup[0] = 1;
      }
    }

    if (!go){
      donot_go_in();
      Serial.println("No db");

      tag_y_db_backup[0] = 0;
    }

    up_firebase(tag_now, go); //envia a tag para firebase

    go = false;
  }
}

void setup (){
  /************************/
  pinMode(GRE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  SPI.begin();          // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522*/
  /************************/

  /************************/
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  if (WiFi.status() != WL_CONNECTED) { //original era while mas if tbm funciona
    delay(200);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  /************************/
}

void loop (){
  if (analogRead(button) >= 1000){
    go_in();

    for(int i = 31; i > 0; i--){
      tag_backup [i] = tag_backup [i - 1];
      tag_db_backup [i] = tag_db_backup [i - 1];
      tag_y_db_backup [i] = tag_y_db_backup [i - 1];
    }

    tag_backup [0] = "button pressed";
    tag_y_db_backup [0] = 0;

    Serial.println("button pressed");
    up_firebase("button pressed", false);

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  else {
    function_RFID();
    server.handleClient();
  }

  /*if(millis() > 30000){
    Serial.println("tchau");
    ESP.restart();
  }*/
}
