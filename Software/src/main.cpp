#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

#include "angel_carol_payer.h"

HardwareSerial Serial1(PB_7, PB_6);

DFRobotDFPlayerMini DFPlayer;
AngelCarolPlayer ACPlayer;

void setup()
{
  pinMode(PA_8, OUTPUT);
  pinMode(PB_10, INPUT);

  Serial1.begin(9600);
  if (!DFPlayer.begin(Serial1)) {
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.  
    }
  }
  DFPlayer.volume(0);  //Set volume value. From 0 to 30
  ACPlayer.begin(&DFPlayer);

}

void loop()
{
  //digitalWrite(PA_8, HIGH);
  //delay(1000);
  //digitalWrite(PA_8, LOW);
  //delay(1000);

  //if(digitalRead(PB_10) == HIGH) {
  //  DFPlayer.play(1);  
  //}

  if(digitalRead(PB_10) == LOW) {
    ACPlayer.insertCoin();
  }

  ACPlayer.process();

}