#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>

#include "angel_carol_payer.h"
#include "board_def.h"


#ifdef STM32C0xx
	HardwareSerial Serial1(PB_7, PC_14);
#else
	HardwareSerial Serial1(PB_7, PB_6);
#endif


DFRobotDFPlayerMini DFPlayer;
AngelCarolPlayer ACPlayer;

volatile int coinCounter = 0;

void coinIrq()
{
	if(digitalRead(COIN_DET) == HIGH) // re read to filter spikes
		coinCounter++;
}

int getCoinCount(void)
{
	volatile int ret;

    noInterrupts();
		ret = coinCounter;
		coinCounter = 0;
    interrupts();

	return ret;
}



void setup()
{
  Serial1.begin(9600);
  if (!DFPlayer.begin(Serial1)) {
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.  
    }
  }
  DFPlayer.volume(0);  //Set volume value. From 0 to 30
  DFPlayer.loop(3); // set to random mode


  pinMode(BOARD_LED, OUTPUT); // hart beat
  pinMode(RELAY, OUTPUT);
  pinMode(VOL_SET, INPUT);
  pinMode(TIME_SET, INPUT);

  pinMode(COIN_DET, INPUT);
  attachInterrupt(COIN_DET,coinIrq,RISING);


  interrupts(); // enable interrupts
}

void loop()
{

  digitalWrite(BOARD_LED, HIGH);

  uint32_t volume_set    = analogRead(VOL_SET)/34; // (0-1023)/34 = 0-30
  uint32_t max_play_time = analogRead(TIME_SET);// 0-1023


  static uint32_t previous_volume = 0;
  static uint32_t play_stop_at = -1;
  static uint32_t play_start_at = -1;


  if(volume_set>30)
	  volume_set = 30;

  if(previous_volume!=volume_set)
  {
	  DFPlayer.volume(volume_set);
  	  previous_volume=volume_set;
  }


  if(getCoinCount()) {
	  digitalWrite(RELAY, HIGH);
	  play_stop_at = -1;
	  play_start_at = millis();
	  DFPlayer.next();
  }

  if(play_start_at != -1)
  {
	  if( (millis()-play_start_at)/1000> max_play_time/17) // max about 1min
	  {
		  DFPlayer.stop();
		  play_start_at = -1;
	  }

  }


  if (DFPlayer.available())
  {
	if(DFPlayer.readType() == DFPlayerPlayFinished)
	{
		play_stop_at = millis();
	}
  }

  if(play_stop_at != -1) // for giving some extra time to put coin, an to prevent fast on/off
  {
	  if(millis()-play_stop_at > 5000)
		  digitalWrite(RELAY, LOW);
  }


  digitalWrite(BOARD_LED, LOW);
  delay(100);


}
