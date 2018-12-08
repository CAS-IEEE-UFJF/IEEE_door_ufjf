//https://gist.github.com/42Bots/54711c9536ef69afad7f3b9aacebb942 (modificado 08/12/2018)

//https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftout/

#include <Arduino.h>

int latchPin = 1;  // pin TX on NodeMCU boards to 74HC595 pin 12
int clockPin = 15; // pin D8 on NodeMCU boards to 74HC595 pin 11
int dataPin = 3;   // pin RX on NodeMCU boards to 74HC595 pin 14

void setup (){
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop (){
  /*aleatoria: forma == a
  bits: forma == b
  contador binario: forma == c
  run == d //melhor exemplo :D
  */
  char forma = 'd';

  //forma aleatoria
  if(forma == 'a'){
    int r = int(map(random(0, 255), 0, 255, 0, 255));
    r = constrain(r, 0, 255);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, r);
    digitalWrite(latchPin, HIGH);
    delay(200);
  }

  //forma de bits
  else if(forma == 'b'){
    digitalWrite(latchPin, LOW);
    //LSBFIRST ou MSBFIRST
    shiftOut(dataPin, clockPin, LSBFIRST, B10101010);
    digitalWrite(latchPin, HIGH);
    delay(500);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, B11111111);
    digitalWrite(latchPin, HIGH);
    delay(500);

    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, B00000000);
    digitalWrite(latchPin, HIGH);
    delay(500);
  }


  //contador binario
  else if(forma == 'c'){
    for (int i = 0; i <= 255; i++) {
      digitalWrite(latchPin, LOW);
      // shift out the bits:
      shiftOut(dataPin, clockPin, MSBFIRST, i);
      //take the latch pin high so the LEDs will light up:
      digitalWrite(latchPin, HIGH);
      // pause before next value:
      delay(500);
    }
  }

  //run
  else if(forma == 'd'){
    int vetplus [] = {B00000000,
                      B00000001,
                      B00000010,
                      B00000100,
                      B00001000,
                      B00010000,
                      B00100000,
                      B01000000,
                      B10000000};

    int vetless [] = {B00000000,
                      B10000000,
                      B01000000,
                      B00100000,
                      B00010000,
                      B00001000,
                      B00000100,
                      B00000010,
                      B00000001};

    for(int i = 0; i < 9; i++){
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, vetplus[i]);
      digitalWrite(latchPin, HIGH);
      delay(50);
    }

    for(int i = 0; i < 9; i++){
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, vetless[i]);
      digitalWrite(latchPin, HIGH);
      delay(50);
    }
  }
}
