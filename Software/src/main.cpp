#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

#include "angel_carol_payer.h"

HardwareSerial Serial1(PB_7, PB_6);

DFRobotDFPlayerMini DFPlayer;
AngelCarolPlayer ACPlayer;

void setup()
{

  Serial1.begin(9600);
  if (!DFPlayer.begin(Serial1)) {
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.  
    }
  }
  DFPlayer.volume(0);  //Set volume value. From 0 to 30
  ACPlayer.begin(&DFPlayer);

  pinMode(PA_8, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PB_10, INPUT);


}

void loop()
{



  //if(digitalRead(PB_10) == HIGH) {
  //  DFPlayer.play(1);  
  //}

  if(digitalRead(PB_10) == LOW) {
    ACPlayer.insertCoin();
  }

  ACPlayer.process();

}