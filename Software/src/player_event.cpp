#include "player_event.h"

PlayerEvent::PlayerEvent(INTERNAL_EVENTS in_event, int in_track, int in_duration)
{
    event = in_event;
    track = in_track;
    duration = in_duration;
}


PlayerEvent::~PlayerEvent()
{

}

void PlayerEvent::start() {
    start_time = millis();
}

void PlayerEvent::terminate() 
{
    duration = 0;
}

int PlayerEvent::getProgress (int max_value, int reverse) {
    int elpasserd = millis() - start_time;

    if(elpasserd > duration) {
        if(reverse)
            return 0;
        return max_value;
    }

    if(duration == 0 ){
        if(reverse)
            return 0;
        return max_value;
    }

    int progress =  elpasserd * max_value / duration;

    if (reverse) {
        return max_value - progress;
    }

    return progress;
}


int PlayerEvent::isPending()
{
    int elpasserd = millis() - start_time;

    if(elpasserd > duration)
        return 0;

    return 1;

}

INTERNAL_EVENTS PlayerEvent::getType()
{
    return event;
}

int PlayerEvent::getTrack()
{
    return track;
}