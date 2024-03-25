#ifndef PHONE
#define PHONE

#include <EEPROM.h>
#include "eeprom_layout.hpp"
#include "at_sender.hpp"

class Phone
{
private:
    enum modes  {
        NOTHING,
        ADDING_CONTACT_ID,
        ADDING_CONTACT_PHONE,
        CALLING_CONTACT,
        CALLING_NUMBER,
        READING_CONTACT,
        CALLING,
    };
    int mode;
    int buffer[10];
    int contact_id;
    int index;
    AtSender *at_sender;
    EepromLayout eeprom_layout;

    void call();
public:
    Phone(AtSender *at_sender);
    void stop();
    void add_contact();
    void call_contact();
    void call_number();
    void read_contact();
    bool enter_number(int n);
    void accept_call();
};

#endif