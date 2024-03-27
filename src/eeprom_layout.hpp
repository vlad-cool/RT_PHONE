#ifndef EEPROM_LAYOUT
#define EEPROM_LAYOUT

struct EepromLayout
{
    unsigned char contacts[10][10];
    unsigned char volume_level;
    bool echo;
};

#endif
