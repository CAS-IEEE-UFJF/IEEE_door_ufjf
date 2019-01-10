/*******************************/
/*      atualizacao            */
/*       10/01/2019            */
/*******************************/

/*****library_str******/ //str == start
#include <Arduino.h> //tem que importar pois estou usando platformIO

#include <ESP8266WiFi.h>  //na real mesmo tem biblioteca aqui que não esta sendo usada mas deixa assim mesmo
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>

#include <FirebaseArduino.h> //o biblioteca problematica. só de importar já da erro

#include <SPI.h> //protocol SPI
#include <MFRC522.h> //RFID
/*****library_end******/

/************************/
#define RST_PIN 5  //GPIO05 ou D1 no nodeMCU, Reset no RFID
#define SS_PIN 16  //GPIO15 ou D0 no nodeMCU, SDA no RFID
                   //GPIO12 ou D6 no nodeMCU, MISO no RFID
                   //GPIO13 ou D7 no nodeMCU, MOSI no RFID
                   //GPIO14 ou D5 no nodeMCU, SCK no RFID

MFRC522 mfrc522(SS_PIN, RST_PIN);

#define RED_LED D4
#define GRE_LED D3
#define BUZZER  D2

const int button = A0;
/************************/

/************************/
const char* ssid = "";      //nome da rede
const char* password = "";  //senha da rede

#define FIREBASE_HOST ""   //autenticação no firebase
#define FIREBASE_AUTH ""

ESP8266WebServer server(80);  //porta 80 é padrão para entrada e saída de dados
/************************/

/************************/
String tag_now = "";
int tag_db = 0;

class membro_class {
  public:
    String UID, nome;

    boolean dev = false,
            cas = false;

    membro_class(String UID_aux, String nome_aux) {
      UID = UID_aux;
      nome = nome_aux;
    }
};

int tam = 26; //tam db
membro_class membro[] = {
  membro_class("B5 80 13 AB", "Tag azul de segurança")
};

void pessoas_especiais (){
  //encontrar a pessoa e colocar verdadeiro nas coisas que ela faz
  membro[0].dev = true;
  membro[1].cas = true;
}

String tag_backup[32] = ""; //codigo da tag
int tag_db_backup[32];      //posicao da tag
int tag_y_db_backup[32];    //está no db?
/************************/

/*******server_str*******/
void handleRoot (){
  //html mega complexo...hahaha
  String textHTML;

  textHTML += "<!DOCTYPE HTML>";
  textHTML += "<html>";
    textHTML += "<head>";
      textHTML += " <meta http-equiv='refresh' content='3'>";
      textHTML += "<title>ESP8266 Server</title>";
      textHTML += "<style>";
        textHTML += "body{background-color: #efefef;}";
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
        textHTML += membro[tag_db_backup[i]].nome;
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
/*******server_end*******/

void go_in (){ //efeito normal de entrada (go_in)
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

void go_in_DEV (){ //efeito diferenciado de entrada (go_in_DEV) para developer
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

  Serial.println("developer");
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

void up_firebase (String tag, boolean go_db){ //enviar as tag para firebase
  //desativado pois o rotiador próprio do ramo não está conectando a internet

  Serial.println("------------------");
  if(go_db)
    Firebase.pushString("tag_read", tag + ", "); //+ name); falta o nome da pessoa
  else
    Firebase.pushString("tag_read", tag + ", " + "unknown");

  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());
  }

  Serial.print("firebase: tag now: ");
  Serial.println(tag);
  Serial.println("------------------");
}

void function_RFID (){
  /*essa funcao faz leitura do RFID e verifica
    se a tag esta no banco de dados. se sim, chama
    a funcao go_in ou go_in_DEV se for tag especial(developer)
  */
  //é confuso mas funciona

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
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "01 D1 FC 52" || "03 1C A0 F6"){ //??
    for(int i = 31; i > 0; i--){
      tag_backup [i] = tag_backup [i - 1];
      tag_db_backup [i] = tag_db_backup [i - 1];
      tag_y_db_backup [i] = tag_y_db_backup [i - 1];
    }

    boolean go = false; //var aux para seber se a tag esta no db
    tag_now = conteudo.substring(1);

    tag_backup[0] = tag_now;
    Serial.println(tag_now);

    for(int i = 0; i < tam; i++){
      if (membro[i].UID == tag_now){
        Serial.println("Yes db");

        if (membro[i].dev) //eh developer
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
    go = !go;
  }
}

void setup (){
  pessoas_especiais();

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
  mfrc522.PCD_Init();
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  delay(25);

  if (analogRead(button) >= 1000){ //se o botão foi presionado. Isso significa que tem gente dentro do ramo querendo sair
    go_in(); //abrir a porta

    for(int i = 31; i > 0; i--){ //atualizar db
      tag_backup [i] = tag_backup [i - 1];
      tag_db_backup [i] = tag_db_backup [i - 1];
      tag_y_db_backup [i] = tag_y_db_backup [i - 1];
    }

    tag_backup [0] = "button pressed";
    tag_y_db_backup [0] = 0;

    Serial.println("button pressed");
    up_firebase("button pressed", false);

    /*********/
    Serial.print("IP address: "); //mostrar qual o IP do esp na porta serial. para developer
    Serial.println(WiFi.localIP());
  }

  else {
    function_RFID();
    server.handleClient();
  }
}
