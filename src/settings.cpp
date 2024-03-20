#include "settings.hpp"

Settings::Settings(AtSender *at_sender)
{
    mode = NOTHING;
    EEPROM.get(0, eeprom_layout);
    this->at_sender = at_sender;
}

void Settings::stop()
{
    mode = NOTHING;
    EEPROM.get(0, eeprom_layout);
}

bool Settings::get_echo()
{
    return eeprom_layout.echo;
}

int Settings::get_volume_level()
{
    return eeprom_layout.volume_level;
}

void Settings::enter_number(int n)
{
    switch (mode)
    {
    case NOTHING:
        switch (n)
        {
        case 1:
            mode = ENTERING_ECHO;
            break;
        case 2:
            mode = ENTERING_VOLUME_0;
            break;
        default:
            break;
        }
        break;
    case ENTERING_ECHO:
        if (n == 0)
        {
            eeprom_layout.echo = false;
        }
        else
        {
            eeprom_layout.echo = true;
        }
        EEPROM.put(0, eeprom_layout);
        at_sender->play_local_melody("1,1,1,2,3"); // Success
        mode = NOTHING;
    case ENTERING_VOLUME_0:
        volume = n;
        mode = ENTERING_VOLUME_1;
        break;
    case ENTERING_VOLUME_1:
        eeprom_layout.volume_level = volume * 10 + n + 1;
        EEPROM.put(0, eeprom_layout);
        at_sender->play_local_melody("1,1,1,2,3"); // Success
        mode = NOTHING;
        break;
    default:
        break;
    }
}
