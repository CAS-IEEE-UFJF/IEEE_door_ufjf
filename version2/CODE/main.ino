/*******************************/
/*      atualização            */
/*       19/01/2019            */
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

#define RED_LED D4 //os pinos de controle do motor também estão ligados aqui(ou seja a ponte H e os LED vermelho e verde tem na verdade o mesmo pino) pois não tem 2 pinos sobrando só para eles
#define GRE_LED D3

#define BUZZER  D2

const int button = A0;
/************************/

/************************/
/*
const char* ssid = "";      //nome da rede
const char* password = "";  //senha da rede
*/

const char* ssid = "";      //nome da rede
const char* password = "";  //senha da rede
/*
const char* ssid = "h'(x)";      //nome da rede
const char* password = "T5e5L0e9C7o7M0u2N7i4C4a0C6o4E0s";  //senha da rede
*/

#define FIREBASE_HOST ""   //autenticação no firebase
#define FIREBASE_AUTH ""

ESP8266WebServer server(80);  //porta 80 é padrão para entrada e saída de dados
/************************/

/************************/
String tag_now = "";
int tag_db = 0;

class membro_class
{
  public:
    String UID, nome;
    boolean   dev = false; //grupos em que a pessoa está. Por padrão vem setado como falso

    bool coisas [27] = {false};
    /*
       0   false, //CAS: presidente
       1   false, //RAS: presidente
       2   false, //PES: presidente
       3   false, //IAS: presidente
       4   false, //SIGHT: presidente
       5   false, //WIE: presidente
       6   false, //SPEAK: presidente
       7   false, //QUALIDADE: presidente
       8   false, //MKT: presidente
       9   false, //EVENTOS: presidente
       10  false, //TI: presidente
       11  false, //CONSELHO: presidente
       12  false, //GP: presidente
       13  false, //PROFESSOR: presidente

       14  false, //CAS: membro
       15  false, //RAS: membro
       16  false, //PES: membro
       17  false, //IAS: membro
       18  false, //SIGHT: membro
       19  false, //WIE: membro
       20  false, //SPEAK: membro
       21  false, //QUALIDADE: membro
       22  false, //MKT: membro
       23  false, //EVENTOS: membro
       24  false, //TI: membro
       25  false, //CONSELHO: membro
       26  false, //GP: membro

       27 no total
  */

    membro_class(String UID_aux, String nome_aux)
    {
      UID = UID_aux;
      nome = nome_aux;
    }
};

int tam_DB = 26; //tam db //<---------- alterar aqui!!!!!!!
membro_class membro[] =
{
};

String tag_backup[32] = ""; //código da tag
int tag_db_backup[32];      //posição da tag
int tag_y_db_backup[32];    //está no db?

boolean meeting = false; //modo reunião
int cont_meeting = 0; //contador para reunião, var aux
int quem_ativou = 200;
double time_now = 0;
double time_aux = 0;


void oque_a_pessoa_faz ()
{ //se a pessoa é desenvolvedor...
  //encontrar a pessoa e colocar verdadeiro(true) nas coisas que ela faz

}
/************************/

/*******server_str*******/
void handleRoot ()
{//html mega complexo...hahaha
  String textHTML;

  textHTML += "<!DOCTYPE HTML>";
  textHTML += "<html>";
    textHTML += "<head>";
      textHTML += " <meta http-equiv='refresh' content='3'>";
      textHTML += "<meta name'viewport' content='width=device-width, initial-scale=1.0'>";
      textHTML += "<title>ESP8266 Server</title>";
      textHTML += "<style>";
        textHTML += "body{background-color: #efefef;}";
      textHTML += "</style>";
    textHTML += "</head>";

    textHTML += "<body>";
      textHTML += "<center>";
      textHTML += "<h1>TAG</h1>";

      /************************/
      for(int i = 0; i < 32; i++)
      {
        textHTML += tag_backup[i]; //UID
        textHTML += ", ";

        if(tag_y_db_backup[i] == 1)
        {
        textHTML += membro[tag_db_backup[i]].nome;
        textHTML += "<br>";
        }
        else
        {
          textHTML += "unknown<br>";
        }
      }
      /************************/

    textHTML += "</body>";
  textHTML += "</html>";

  server.send(200, "text/html", textHTML);
}

void handleNotFound ()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}
/*******server_end*******/

void go_in ()
{ //efeito normal de entrada (go_in)
  digitalWrite(GRE_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  /*************************/

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

  /*************************/
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

void go_in_DEV ()
{ //efeito diferenciado de entrada (go_in_DEV) para developer
  digitalWrite(GRE_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  /*************************/

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

  /*************************/
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


/******************************/
//minha música de star wars hehe
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

int counter = 0;

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(BUZZER, note);
  delay(duration);

  //Stop tone on buzzerPin
  noTone(BUZZER);

  delay(50);

  //Increment counter
  counter++;
}

void firstSection()
{
  beep(a, 500);
  beep(a, 500);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  /*
  delay(500);

  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);
  */
}

void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);
  beep(fSH, 250);

  delay(325);

  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);
  beep(cSH, 175);
  beep(cH, 125);
  beep(b, 125);
  beep(cH, 250);

  delay(350);
}

void go_in_DEV_star_wars ()
{
  digitalWrite(GRE_LED, HIGH);
  digitalWrite(RED_LED, LOW);
  /*************************/

  //Play first section
  firstSection();

  //Play second section
  //secondSection();

  /*************************/
  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, LOW);

  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  delay(100);
  digitalWrite(GRE_LED, LOW);
  digitalWrite(RED_LED, LOW);

  Serial.println("developer");
}
/******************************/

void donot_go_in ()
{ //tag nao cadastrada
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

void meeting_led ()
{ //piscar led vermelho para indicar que está em reunião
  digitalWrite(RED_LED, HIGH);
  digitalWrite(GRE_LED, HIGH);
  delay(250);
  digitalWrite(RED_LED, LOW);
  digitalWrite(GRE_LED, LOW);
  delay(100);
}

void up_firebase (String tag, boolean go_db)
{ //enviar as tag para firebase
  //desativado pois o roteador próprio do ramo não está conectando a internet

  Serial.println("-------firebase str-----------");
  if(go_db)
    Firebase.pushString("tag_read", tag + ", "); //+ name); falta o nome da pessoa
  else
    Firebase.pushString("tag_read", tag + ", " + "unknown");

  if (Firebase.failed())
  {
    Serial.print("setting /number failed:");
    Serial.println(Firebase.error());
  }

  Serial.print("firebase: tag now: ");
  Serial.println(tag);
  Serial.println("--------firebase end----------");
}

void function_RFID ()
{
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
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "01 D1 FC 52" || "03 1C A0 F6")
  { //não sei o motivo de ser entre esses dois valores
    for(int i = 31; i > 0; i--)
    {
      tag_backup [i] = tag_backup [i - 1];
      tag_db_backup [i] = tag_db_backup [i - 1];
      tag_y_db_backup [i] = tag_y_db_backup [i - 1];
    }

    boolean go = false; //var aux para seber se a tag esta no db
    tag_now = conteudo.substring(1);

    tag_backup[0] = tag_now;
    Serial.println(tag_now);

    if(meeting)
    {
      Serial.println("modo reuniao");

      for (int ddb = 0; ddb < tam_DB; ddb++)
      {//varrer db
       if (membro[ddb].UID == tag_now)
        {//se tiver no db
          Serial.println("Yes db");

          for(int va = 0; va < 14; va++)
          {//varrer todos os presidentes //para desativar
            if (membro[ddb].coisas[va])
            {//se for presidente então cont_meeting = cont_meeting + 1
              go_in();

              cont_meeting += 1;
              if(cont_meeting == 1)
                time_now = millis() - time_aux;

              go = true;
            }
          }

          if(!go)
          {
            for (int ddbb = 0; ddbb < tam_DB; ddbb++)
            {
              if (membro[ddbb].coisas[quem_ativou])
              {
                go_in();

                go = true;
              }
            }
          }
        }
      }

      if(go == false)
      {
        donot_go_in();
      }
      else{
        go = false;
      }
    }

    else
    {
      for(int i = 0; i < tam_DB; i++)
      {
       if (membro[i].UID == tag_now)
        {
          Serial.println("Yes db");

          for(int va = 0; va < 14; va++)
          {//varrer todos os presidentes
            if (membro[i].coisas[va])
            {//se for presidente então cont_meeting = cont_meeting + 1
              cont_meeting += 1;

              if(cont_meeting > 2)
              {
                quem_ativou = va + 14;
              }
            }
          }

          if (membro[i].dev) //eh developer
          {
           if ((membro[i].UID == "FD 3C 50 C5") || //eu quero que minha entrada seja triunfal
               (membro[i].UID == "51 4F E2 08"))
            {
              go_in_DEV_star_wars();
            }
            else
             go_in_DEV();
         }
         else
            go_in();


         tag_db_backup[0] = i;
         go = true;
         tag_y_db_backup[0] = 1;
        }
      }

      if (!go)
      {
       donot_go_in();
       Serial.println("No db");

        tag_y_db_backup[0] = 0;
      }

      up_firebase(tag_now, go); //envia a tag para firebase
      go = !go;
    }
  }
}

void setup ()
{
  oque_a_pessoa_faz();

  /************************/
  pinMode(GRE_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  SPI.begin();          // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522*/
  /************************/

  /************************/
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  if (WiFi.status() != WL_CONNECTED)
  { //original era while mas if tbm funciona
    delay(200);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []()
  {
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  /************************/
}

void loop ()
{
  mfrc522.PCD_Init();
  mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_max);
  delay(25);

  if (analogRead(button) >= 1000)
  { //se o botão foi presionado. Isso significa que tem gente dentro do ramo querendo sair
    go_in(); //abrir a porta

    for(int i = 31; i > 0; i--)
    { //atualizar db
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

    if((cont_meeting > 2) && (time_now <= 10000)) //10segundos
    {
      meeting = !meeting;
      cont_meeting = 0;
    }

    Serial.println(cont_meeting);
    Serial.println("   ");
    Serial.println(meeting);
  }

  else
  {
    function_RFID();
    server.handleClient();
  }


  if(meeting)
  {
    meeting_led();
  }

  if(cont_meeting == 0)
    time_aux = millis();
}
