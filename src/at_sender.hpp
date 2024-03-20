#ifndef AT_SENDER
#define AT_SENDER

#include <Arduino.h>

class AtSender
{
private:
    int volume_level;
public:
    AtSender();
    AtSender(int volume_level);
    void call(String s);
    void set_volume_level(int volume_level);
    void play_local_sound(String s);
    void play_local_melody(String s);
    void wait_playing();
};

#endif