#ifndef PLAYER_EVENT_H
#define PLAYER_EVENT_H

#include <Arduino.h>

enum INTERNAL_EVENTS{
    EVT_PLAY,
    EVT_FADE_IN,
    EVT_FADE_OUT,
    EVT_STOP_PLAYING,
};

class PlayerEvent
{
private:
    INTERNAL_EVENTS event;
    int track;
    unsigned int duration;
    unsigned int start_time;
    int is_terminated;
public:
    PlayerEvent(INTERNAL_EVENTS event, int track, int duration);
    ~PlayerEvent();
    void start();
    void terminate();
    int getProgress(int max_value, int reverse);
    int isPending();
    INTERNAL_EVENTS getType();
    int getTrack();
};

#endif //PLAYER_EVENT_H