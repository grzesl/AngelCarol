#ifndef ANGEL_CAROL_PLAYER_H
#define ANGEL_CAROL_PLAYER_H

#include <list>
#include <player_event.h>

class DFRobotDFPlayerMini;

class AngelCarolPlayer
{
private:
    DFRobotDFPlayerMini *dfplayer;
    PlayerEvent * currentEvent;
    std::list <PlayerEvent*> events;
    int max_volume; //30 max
    int max_carol_duration_ms;
    int max_intro_duration_ms;
    int fade_out_duration_ms;
    int fade_in_duration_ms;
    int current_track = 0; //none
    int max_track_no;
    unsigned int last_insert_coint_time;
    int curr_volume;

public:
    AngelCarolPlayer(/* args */);
    ~AngelCarolPlayer();
    void insertCoin();
    void process();
    void setVolume(int volume);
    void setCarolDuration(int duration_ms);
    int suffle();
    void begin(DFRobotDFPlayerMini * in_dfplayer);
    void playIntro();
    void playCarol(int track_no);
    int isPlaying();

};




#endif //#ifndef ANGEL_CAROL_PLAYER_H