//fazendo leitura na marra
//ainda em desenvolvimento
//parece que algumas teclas agarram

#include <Arduino.h>

const int l1 = 0;
const int l2 = 12;
const int l3 = 14;
const int l4 = 2;

const int c1 = 16;
const int c2 = 4;
const int c3 = 5;
const int c4 = 13;

void desligar (){
  digitalWrite(l1, LOW);
  digitalWrite(l2, LOW);
  digitalWrite(l3, LOW);
  digitalWrite(l4, LOW);
}

void pesquisar (){
  if (digitalRead(c1) == HIGH){
    Serial.println("c1");
  }

  if (digitalRead(c2) == HIGH){
    Serial.println("c2");
  }

  if (digitalRead(c3) == HIGH){
    Serial.println("c3");
  }

  if (digitalRead(c4) == HIGH){
    Serial.println("c4");
  }
}

void setup (){
  Serial.begin(115200);

  //Pinos ligados aos pinos 1, 2, 3 e 4 do teclado - Linhas
  pinMode(l1, OUTPUT);
  pinMode(l2, OUTPUT);
  pinMode(l3, OUTPUT);
  pinMode(l4, OUTPUT);

  //Pinos ligados aos pinos 5, 6, 7 e 8 do teclado - Colunas
  pinMode(c1, INPUT);
  pinMode(c2, INPUT);
  pinMode(c3, INPUT);
  pinMode(c4, INPUT);

  Serial.begin(115200);
  Serial.println("Aguardando acionamento das teclas...");
}

void loop (){
  desligar();
  digitalWrite(l1, HIGH);
  pesquisar();

  delay(50);

  desligar();
  digitalWrite(l2, HIGH);
  pesquisar();

  delay(50);

  desligar();
  digitalWrite(l3, HIGH);
  pesquisar();

  delay(50);

  desligar();
  digitalWrite(l4, HIGH);
  pesquisar();

  delay(100);
}

//usando a biblioteca keypad :D
/*//https://techtutorialsx.com/2017/03/18/esp8266-interfacing-with-a-4x4-matrix-keypad/ (modificado 08/12/2018)

#include <Arduino.h>
#include <Keypad.h>

const byte n_rows = 4;
const byte n_cols = 4;

char keys[n_rows][n_cols] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte colPins[n_rows] = {D3, D2, D1, D0};
byte rowPins[n_cols] = {D7, D6, D5, D4};

Keypad myKeypad = Keypad( makeKeymap(keys), rowPins, colPins, n_rows, n_cols);

void setup(){
  Serial.begin(115200);
}

void loop(){
  char myKey = myKeypad.getKey();

  if (myKey != NULL){
    Serial.print("Key pressed: ");
    Serial.println(myKey);
  }
}
*/
