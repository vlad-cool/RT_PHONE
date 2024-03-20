#ifndef PHONE
#define PHONE

#include <EEPROM.h>
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
    };
    int mode;
    int buffer[10];
    int contact_id;
    int index;
    AtSender *at_sender;

    void call();
public:
    Phone(AtSender *at_sender);
    void add_contact();
    void call_contact();
    void call_number();
    bool enter_number(int n);
};

#endif