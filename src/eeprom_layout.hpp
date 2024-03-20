#ifndef EEPROM_LAYOUT
#define EEPROM_LAYOUT

struct EepromLayout
{
    int contacts[10][10];
    int volume_level;
    bool echo;
};

#endif
