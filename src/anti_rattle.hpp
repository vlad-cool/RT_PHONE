#ifndef ANTI_RATTLE
#define ANTI_RATTLE

#include <Arduino.h>

class AntiRattle
{
private:
    unsigned long min_dt;
    unsigned long last_update;
public:
    AntiRattle();
    AntiRattle(unsigned long min_dt);
    bool test();
};

#endif