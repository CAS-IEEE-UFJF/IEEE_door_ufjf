//fazendo leitura na marra
//ainda em desenvolvimento

/*parece que algumas teclas agarram.
  obs: o problema foi corrigido colocando resistor e ground nos pinos de leitura. equivalente a button push_down*/

#include <Arduino.h>

//olhando os pinos da matriz de frente. O primeiro pino da esquerda é o 1 o segundo é o 2 e assim por diante ate o oitavo pino

const int l1 = 0;  //D3 in nodeMCU and 1 in keypad
const int l2 = 12; //D6 in nodeMCU and 2 in keypad
const int l3 = 14; //D5 in nodeMCU and 3 in keypad
const int l4 = 2;  //D4 in nodeMCU and 4 in keypad

                   //colocar ground (com resistor)
const int c1 = 16; //D0 in nodeMCU and 5 in keypad
const int c2 = 4;  //D2 in nodeMCU and 6 in keypad
const int c3 = 5;  //D1 in nodeMCU and 7 in keypad
const int c4 = 13; //D7 in nodeMCU and 8 in keypad

void desligar (){
  digitalWrite(l1, LOW);
  digitalWrite(l2, LOW);
  digitalWrite(l3, LOW);
  digitalWrite(l4, LOW);
}

void pesquisar (int l){
  if (digitalRead(c1) == HIGH){
    if(l == 1)
      Serial.print("1  ");
    if(l == 2)
      Serial.print("4  ");
    if(l == 3)
      Serial.print("7  ");
    if(l == 4)
      Serial.print("*  ");

    while(digitalRead(c1) == HIGH){
      //travar code
    }
  }

  if (digitalRead(c2) == HIGH){
    if(l == 1)
      Serial.print("2  ");
    if(l == 2)
      Serial.print("5  ");
    if(l == 3)
      Serial.print("8  ");
    if(l == 4)
      Serial.print("0  ");

    while(digitalRead(c2) == HIGH){
      //travar code
    }
  }

  if (digitalRead(c3) == HIGH){
    if(l == 1)
      Serial.print("3  ");
    if(l == 2)
      Serial.print("6  ");
    if(l == 3)
      Serial.print("9  ");
    if(l == 4)
      Serial.print("#  ");

    while(digitalRead(c3) == HIGH){
      //travar code
    }
  }

  if (digitalRead(c4) == HIGH){
    if(l == 1)
      Serial.print("A  ");
    if(l == 2)
      Serial.print("B  ");
    if(l == 3)
      Serial.print("C  ");
    if(l == 4)
      Serial.print("D  ");

    while(digitalRead(c4) == HIGH){
      //travar code
    }
  }

  Serial.println(" ");
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
  pesquisar(1);

  delay(50);

  desligar();
  digitalWrite(l2, HIGH);
  pesquisar(2);

  delay(50);

  desligar();
  digitalWrite(l3, HIGH);
  pesquisar(3);

  delay(50);

  desligar();
  digitalWrite(l4, HIGH);
  pesquisar(4);

  delay(100);
}

/**********************************/

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
