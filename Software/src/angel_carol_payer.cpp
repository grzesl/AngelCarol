#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include "angel_carol_payer.h"

AngelCarolPlayer::AngelCarolPlayer(/* args */)
{
    //default values;
    currentEvent = NULL;
    max_volume = 30; //30 max
    max_carol_duration_ms = 30000; //30 sec
    max_intro_duration_ms = 10000; //10 sec
    fade_out_duration_ms = 2000; //2 sec
    fade_in_duration_ms = 1000; //1 sec
    current_track = 0; //none
    max_track_no = 1;
    last_insert_coint_time = 0;

    curr_volume = 0;
}

AngelCarolPlayer::~AngelCarolPlayer()
{

}


void AngelCarolPlayer::insertCoin()
{
    unsigned int elpassed = millis() - last_insert_coint_time;
    if(elpassed < 2000)
        return;

    last_insert_coint_time = millis();

    current_track = suffle();
    playCarol(current_track);
}

void AngelCarolPlayer::process()
{
    int new_volume = 0;
    if(currentEvent == NULL && events.size() > 0)
    {
        currentEvent = events.front();
        events.pop_front();
        currentEvent->start();
        if(currentEvent->getType() == EVT_FADE_IN) {
            dfplayer->play(currentEvent->getTrack());
        }
    }

    if(currentEvent == NULL) {
        return;
    }


    switch(currentEvent->getType()) {
        case EVT_FADE_IN:
            new_volume = currentEvent->getProgress(max_volume, 0);
            if(curr_volume != new_volume){
                for(int i = 0;i < new_volume - curr_volume;i++)
                    dfplayer->volumeUp();
            }
            curr_volume = new_volume;
            
        break;
        case EVT_FADE_OUT:
            new_volume = currentEvent->getProgress(max_volume, 1);
            if(curr_volume != new_volume){
                for(int i = 0;i < curr_volume - new_volume;i++)
                    dfplayer->volumeDown();
            }
            curr_volume = new_volume;

           if(!currentEvent->isPending())
                dfplayer->stop();
        break;
        case EVT_PLAY:
           // dfplayer->play(currentEvent->getTrack());
        break;
        default:
            dfplayer->stop();
        break;
    }


    if(!currentEvent->isPending()) {
        delete currentEvent;
        currentEvent = NULL;
    }

}

void AngelCarolPlayer::setVolume(int volume)
{
    max_volume = volume;
}

void AngelCarolPlayer::setCarolDuration(int duration_ms)
{
    max_carol_duration_ms = duration_ms;
}


int AngelCarolPlayer::suffle()
{
    return random(1, max_track_no);
}


void AngelCarolPlayer::begin(DFRobotDFPlayerMini * in_dfplayer)
{
    dfplayer = in_dfplayer;
    randomSeed(analogRead(0));
    playIntro();
}


void AngelCarolPlayer::playIntro()
{
    events.push_back(new PlayerEvent(EVT_FADE_IN, 1, fade_in_duration_ms));
    events.push_back(new PlayerEvent(EVT_PLAY, 1, max_intro_duration_ms));
    events.push_back(new PlayerEvent(EVT_FADE_OUT, 1, fade_out_duration_ms));
}

void AngelCarolPlayer::playCarol(int track_no)
{
    if(isPlaying() && currentEvent->getType() == EVT_PLAY) {
      //mark current event as expired;
      currentEvent->terminate();
    }
    events.push_back(new PlayerEvent(EVT_FADE_IN, track_no, fade_in_duration_ms));
    events.push_back(new PlayerEvent(EVT_PLAY, track_no, max_carol_duration_ms));
    events.push_back(new PlayerEvent(EVT_FADE_OUT, track_no, fade_out_duration_ms));
}

int AngelCarolPlayer::isPlaying()
{
    if(currentEvent!= NULL) 
        return 1;
    return 0;
}