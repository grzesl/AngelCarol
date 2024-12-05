#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include "angel_carol_payer.h"
#include <IWatchdog.h>

AngelCarolPlayer::AngelCarolPlayer(/* args */)
{
    // default values;
    currentEvent = NULL;
    max_volume = 26;   // 30 max
    prev_volume = 0;            
    max_carol_duration_ms = 20000; // 30 sec
    max_intro_duration_ms = 5000;  // 10 sec
    fade_out_duration_ms = 2000;   // 2 sec
    fade_in_duration_ms = 400;     // 1 sec
    current_track = 0;             // none
    max_track_no = 15;
    prev_track_no = 0;
    last_insert_coint_time = 0;

    curr_volume = 0;
}

AngelCarolPlayer::~AngelCarolPlayer()
{
}

void AngelCarolPlayer::insertCoin()
{
    unsigned int elpassed = millis() - last_insert_coint_time;
    if (elpassed < 5000) // must be
        return;
    digitalWrite(PA_8, HIGH);
    last_insert_coint_time = millis();

    current_track = suffle();
    playCarol(current_track);
}

void AngelCarolPlayer::process()
{
    int new_volume = 0;
    if (currentEvent == NULL && events.size() > 0)
    {
        currentEvent = events.front();
        events.pop_front();
        currentEvent->start();
        if (currentEvent->getType() == EVT_FADE_IN)
        {
            dfplayer->play(currentEvent->getTrack());
        }
    }

    if (currentEvent == NULL)
    {
    	IWatchdog.reload();
        return;
    }

    switch (currentEvent->getType())
    {
    case EVT_FADE_IN:
        new_volume = currentEvent->getProgress(max_volume, 0);
        if (curr_volume != new_volume)
            dfplayer->volume(new_volume);

        curr_volume = new_volume;
        break;
    case EVT_FADE_OUT:
        new_volume = currentEvent->getProgress(max_volume, 1);
        if (curr_volume != new_volume)
            dfplayer->volume(new_volume);
        curr_volume = new_volume;

        if (!currentEvent->isPending())
        {
            dfplayer->stop();
            delay(100);
        }
        break;
    case EVT_PLAY:
        // dfplayer->play(currentEvent->getTrack());
    	IWatchdog.reload();
        delay(10);
        if (currentEvent->isPending())
            digitalWrite(PA_8, LOW);
        break;
    default:
        dfplayer->stop();
        delay(100);
        break;
    }

    if (!currentEvent->isPending())
    {
        delete currentEvent;
        currentEvent = NULL;
    }
}

void AngelCarolPlayer::setVolume(int volume)
{
    if (max_volume != volume && 
        prev_volume != volume)
    {
        prev_volume = max_volume;
        max_volume = volume;
        dfplayer->volume(max_volume);
    }
}

void AngelCarolPlayer::setCarolDuration(int duration_ms)
{
    max_carol_duration_ms = duration_ms;
}

int AngelCarolPlayer::suffle()
{   
    int newTrack  = 0;
    do{
        newTrack = random(1, max_track_no);
    } while(newTrack == prev_track_no);
    prev_track_no = newTrack;
    return newTrack;
}

void AngelCarolPlayer::begin(DFRobotDFPlayerMini *in_dfplayer)
{
    dfplayer = in_dfplayer;
    randomSeed(analogRead(0));
    playIntro();
}

void AngelCarolPlayer::playIntro()
{
    events.push_back(new PlayerEvent(EVT_FADE_IN, WELCOME, fade_in_duration_ms));
    events.push_back(new PlayerEvent(EVT_PLAY, WELCOME, max_intro_duration_ms));
    events.push_back(new PlayerEvent(EVT_FADE_OUT, WELCOME, fade_out_duration_ms));
}

void AngelCarolPlayer::playCarol(int track_no)
{
    if (isPlaying() && currentEvent->getType() == EVT_PLAY)
    {
        // mark current event as expired;
        currentEvent->terminate();
    }
    events.push_back(new PlayerEvent(EVT_FADE_IN, GOOD_BLESS_YOU, 100));
    events.push_back(new PlayerEvent(EVT_PLAY, GOOD_BLESS_YOU, 2000));
    events.push_back(new PlayerEvent(EVT_FADE_OUT, GOOD_BLESS_YOU, 100));

    events.push_back(new PlayerEvent(EVT_FADE_IN, track_no, fade_in_duration_ms));
    events.push_back(new PlayerEvent(EVT_PLAY, track_no, max_carol_duration_ms));
    events.push_back(new PlayerEvent(EVT_FADE_OUT, track_no, fade_out_duration_ms));
}

int AngelCarolPlayer::isPlaying()
{
    if (currentEvent != NULL)
        return 1;
    return 0;
}

int AngelCarolPlayer::getMaxCarolDuration()
{
    return max_carol_duration_ms + fade_in_duration_ms + fade_out_duration_ms;
}
