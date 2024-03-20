#ifndef SETTINGS
#define SETTINGS

#include "eeprom_layout.hpp"
#include "at_sender.hpp"
#include "EEPROM.h"

class Settings
{
private:
    enum modes {
        NOTHING,
        ENTERING_ECHO,
        ENTERING_VOLUME_0,
        ENTERING_VOLUME_1,
    };
    int mode;
    int volume;
    EepromLayout eeprom_layout;
    AtSender *at_sender;
public:
    Settings(AtSender *at_sender);
    bool get_echo();
    int get_volume_level();
    void enter_number(int n);
};

#endif