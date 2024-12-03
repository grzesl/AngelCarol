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
static uint32_t stop_relay = 0;

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
  ACPlayer.begin(&DFPlayer);


  pinMode(BOARD_LED, OUTPUT); // hart beat
  pinMode(RELAY, OUTPUT);
  pinMode(VOL_SET, INPUT);
  pinMode(TIME_SET, INPUT);

  pinMode(COIN_DET, INPUT);
  attachInterrupt(COIN_DET,coinIrq,RISING);

	digitalWrite(RELAY, HIGH);
  
  interrupts(); // enable interrupts

  stop_relay = millis() + 60000;
}

void loop()
{
  uint32_t volume_set    = analogRead(VOL_SET)/34; // (0-1023)/34 = 0-30
  uint32_t max_play_time = analogRead(TIME_SET)*60;// 0-1023

  static uint32_t previous_volume = 0;
  static uint32_t previous_duration = 0;
  static uint32_t blink = 0;

  if(volume_set>30)
	  volume_set = 30;

  if(previous_volume!=volume_set)
  {
	  ACPlayer.setVolume(volume_set);
    previous_volume = volume_set;
  }
  
  if(previous_duration!=max_play_time)
  {
  	ACPlayer.setCarolDuration(max_play_time);
    previous_duration = max_play_time;
  } 
  
  if(getCoinCount()) {
	  digitalWrite(RELAY, HIGH);
	  ACPlayer.insertCoin();
    stop_relay = millis() + ACPlayer.getMaxCarolDuration() + 10000;
  }

  if(stop_relay != 0 && stop_relay < millis()) // for giving some extra time to put coin, an to prevent fast on/off
  {
		digitalWrite(RELAY, LOW);
    stop_relay = 0;
  }
  
  ACPlayer.process();

  if(blink % 100 == 0) 
    digitalWrite(BOARD_LED, LOW);
  if(blink % 100 == 50) 
    digitalWrite(BOARD_LED, HIGH);
  blink ++;
}
