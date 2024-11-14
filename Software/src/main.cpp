#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>


//HardwareSerial Serial1(PB_7, PB_6);
DFRobotDFPlayerMini DFPlayer;

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
  DFPlayer.volume(30);  //Set volume value. From 0 to 30
  

}

void loop()
{
  //digitalWrite(PA_8, HIGH);
  //delay(1000);
  //digitalWrite(PA_8, LOW);
  //delay(1000);

  if(digitalRead(PB_10) == HIGH) {
    DFPlayer.play(1);  
  }

}