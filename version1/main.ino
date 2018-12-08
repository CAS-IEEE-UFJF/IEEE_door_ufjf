//este codigo foi descontinuado para dar lugar ao ESP8266

//bibliotecas
#include <SPI.h>
#include <MFRC522.h>
#include <Arduino.h>
//#include <Servo.h>
//#include <Stepper.h>

const int stepsPerRevolution = 200;

//Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

//#define serv 5
//Servo myServo;

//Pinos
#define SS_PIN 10
#define RST_PIN 9

#define LED_VERDE 8
#define LED_VERMELHO 7

#define BUZZER 6

String IDtag = ""; //Variável que armazenará o ID da Tag
bool Permitido = false; //Variável que verifica a permissão

//Vetor responsável por armazenar os ID's das Tag's cadastradas
String TagsCadastradas[] = {"5534652",
                            "fd3c50c5", //wesley
                            "514fe28"   //walmor
                            };

// Cria uma nova instância para o leitor e passa os pinos como parâmetro
MFRC522 LeitorRFID(SS_PIN, RST_PIN);

void efeitoPermitido() {
  //myServo.write(90);

  int qtd_bips = 1; //definindo a quantidade de bips
  for(int j=0; j<qtd_bips; j++){
    //Ligando o buzzer com uma frequência de 528 hz dó e ligando o led verde.
    tone(BUZZER, 528);
    digitalWrite(LED_VERDE, HIGH);
    delay(150);

    //Desligando o buzzer e led verde.
    noTone(BUZZER);
    delay(100);
    digitalWrite(LED_VERDE, LOW);
  }

  delay(500);
  //myServo.write(180);
}

void acessoLiberado(){
  //Exibe a mensagem "Tag Cadastrada" e o ID da tag não cadastrada
  Serial.println("Tag Cadastrada: " + IDtag);
  efeitoPermitido();  //Chama a função efeitoPermitido()
  Permitido = false;  //Seta a variável Permitido como false novamente
}

void efeitoNegado(){
  int qtd_bips = 2;  //definindo a quantidade de bips
  for(int j=0; j<qtd_bips; j++){
    //Ligando o buzzer com uma frequência de 528 hz e ligando o led vermelho.
    tone(BUZZER,528);
    digitalWrite(LED_VERMELHO, HIGH);
    delay(200);

    //Desligando o buzzer e o led vermelho.
    noTone(BUZZER);
    digitalWrite(LED_VERMELHO, LOW);
    //delay(500);
  }
}

void acessoNegado(){
  Serial.println("Tag NAO Cadastrada: " + IDtag);
  //Exibe a mensagem "Tag NAO Cadastrada" e o ID da tag cadastrada
  efeitoNegado(); //Chama a função efeitoNegado()
}

void Leitura(){
  IDtag = ""; //Inicialmente IDtag deve estar vazia.

  //Verifica se existe uma Tag presente
  if (!LeitorRFID.PICC_IsNewCardPresent() || !LeitorRFID.PICC_ReadCardSerial() ){
    delay(50);
    return;
  }

  //Pega o ID da Tag através da função LeitorRFID.uid
  //e Armazena o ID na variável IDtag
  for (byte i = 0; i < LeitorRFID.uid.size; i++) {
    IDtag.concat(String(LeitorRFID.uid.uidByte[i], HEX));
    Serial.println(IDtag);
  }

  //Compara o valor do ID lido com os IDs armazenados
  //no vetor TagsCadastradas[]
  for (int i = 0; i < (sizeof(TagsCadastradas)/sizeof(String)); i++) {
    if(  IDtag.equalsIgnoreCase(TagsCadastradas[i])  ){
      //Variável Permitido assume valor verdadeiro
      //caso o ID Lido esteja cadastrado
      Permitido = true;
    }
  }

  if(Permitido == true)
    acessoLiberado(); //Se a variável Permitido for verdadeira será chamada a função acessoLiberado()
  else
    acessoNegado(); //Se não será chamada a função acessoNegado()

  delay(2000); //aguarda 2 segundos para efetuar uma nova leitura
}

void setup() {
  Serial.begin(9600);             // Inicializa a comunicação Serial
  SPI.begin();                    // Inicializa comunicacao SPI
  LeitorRFID.PCD_Init();          // Inicializa o leitor RFID

  pinMode(LED_VERDE, OUTPUT);     // Declara o pino do led verde como saída
  pinMode(LED_VERMELHO, OUTPUT);  // Declara o pino do led vermelho como saída
  pinMode(BUZZER, OUTPUT);        // Declara o pino do buzzer como saída

  //myServo.attach(serv);
  //myServo.write(90);  //para calibrar

  //myStepper.setSpeed(180);
}

void loop() {
  Leitura();  //Chama a função responsável por fazer a leitura das Tag's
  //myStepper.step(stepsPerRevolution);
  Serial.println("akiq");
}

/**
const int UP = 5;
const int DOWN = 6;
int n = 1;

void setup (){
  Serial.begin(9600);

  pinMode(UP, OUTPUT);
  pinMode(DOWN, OUTPUT);
}

void loop(){
  Serial.println("UP = LOW ____ DOWN = HIGH");
  digitalWrite(UP, LOW);
  digitalWrite(DOWN, HIGH);
  delay(1500);

  digitalWrite(UP, LOW);
  digitalWrite(DOWN, LOW);
  delay(2000);

  Serial.println("UP = HIGH ____ DOWN = LOW");
  digitalWrite(UP, HIGH);
  digitalWrite(DOWN, LOW);
  delay(100);

  analogWrite(UP, LOW);
  analogWrite(DOWN, LOW);
  delay(5000);

  n++;
  Serial.println(n);
}
**/