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
            at_sender->play_local_sound("settings.amr");
            at_sender->play_local_sound("echo.amr");
            break;
        case 2:
            mode = ENTERING_VOLUME_0;
            at_sender->play_local_sound("settings.amr");
            at_sender->play_local_sound("volume.amr");
            break;
        case 0:
            at_sender->play_local_sound("reset.amr");
            mode = RESETTING;
            break;
        default:
            break;
        }
        break;
    case ENTERING_ECHO:
        if (n == 0)
        {
            eeprom_layout.echo = false;
            at_sender->play_local_sound("echo.amr");
            at_sender->play_local_sound("off.amr");
        }
        else
        {
            eeprom_layout.echo = true;
            at_sender->play_local_sound("echo.amr");
            at_sender->play_local_sound("on.amr");
        }
        EEPROM.put(0, eeprom_layout);
        mode = NOTHING;
    case ENTERING_VOLUME_0:
        volume = n;
        mode = ENTERING_VOLUME_1;
        break;
    case ENTERING_VOLUME_1:
        eeprom_layout.volume_level = volume * 10 + n + 1;
        EEPROM.put(0, eeprom_layout);
        at_sender->play_local_sound("volume.amr");
        at_sender->play_local_sound(String(volume / 10) + ".amr");
        at_sender->play_local_sound(String(volume % 10) + ".amr");
        mode = NOTHING;
        break;
    case RESETTING:
        if (n == 0)
        {
            eeprom_layout.echo = false;
            eeprom_layout.volume_level = 99;
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 10; j++)
                {
                    eeprom_layout.contacts[i][j] = 255;
                }
            }
            at_sender->play_local_sound("reset.amr");
            at_sender->play_local_sound("success.amr");
        }
        mode = NOTHING;
    default:
        break;
    }
}
