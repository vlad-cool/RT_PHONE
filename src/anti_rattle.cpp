#include "anti_rattle.hpp"

AntiRattle::AntiRattle()
{
    min_dt = 50;
    last_update = 0;
}

AntiRattle::AntiRattle(unsigned long min_dt)
{
    this->min_dt = min_dt;
    last_update = 0;
}

bool AntiRattle::test()
{
    unsigned long t = millis();
    bool return_value = t - last_update > min_dt;
    last_update = t;
    return return_value;
}
