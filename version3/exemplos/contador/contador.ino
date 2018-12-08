//https://gist.github.com/42Bots/54711c9536ef69afad7f3b9aacebb942 (modificado 08/12/2018)

#include <Arduino.h>

int latchPin = 1; // pin TX on NodeMCU boards
int clockPin = 15; // pin D8 on NodeMCU boards
int dataPin = 3; // pin RX on NodeMCU boards

void setup (){
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop (){
  //contador binario
  for (int i=0; i<=255; i++) {
    digitalWrite(latchPin, LOW);
    // shift out the bits:
    shiftOut(dataPin, clockPin, MSBFIRST, i);
    //take the latch pin high so the LEDs will light up:
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
}
