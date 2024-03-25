#include "phone.hpp"

Phone::Phone(AtSender *at_sender)
{
    mode = NOTHING;
    contact_id = -1;
    index = 0;
    this->at_sender = at_sender;
}

void Phone::stop()
{
    if (mode == CALLING)
    {
        at_sender->drop_call();
    }
    mode = NOTHING;
}

void Phone::call()
{
    String s = "0000000000";
    for (int i = 0; i < 10; i++)
    {
        s[i] += buffer[i];
    }
    at_sender->call(s);
    mode = CALLING;
}

void Phone::accept_call()
{
    at_sender->accept_call();
    mode = CALLING;
}

void Phone::add_contact()
{
    mode = ADDING_CONTACT_ID;
}

void Phone::read_contact()
{
    mode = READING_CONTACT;
}

void Phone::call_contact()
{
    mode = CALLING_CONTACT;
}

void Phone::call_number()
{
    mode = CALLING_NUMBER;
}

bool Phone::enter_number(int n)
{
    switch (mode)
    {
    case ADDING_CONTACT_ID:
        contact_id = n;
        if (contact_id < 0 || contact_id > 9)
        {
            mode = NOTHING;
            at_sender->play_local_melody("3,2,1,1,1"); // Fail
            return true;
        }
        else
        {
            index = -1;
            mode = ADDING_CONTACT_PHONE;
        }
        break;
    case ADDING_CONTACT_PHONE:
        if (contact_id < 0 || contact_id > 9)
        {
            mode = NOTHING;
            at_sender->play_local_melody("3,2,1,1,1"); // Fail
            return true;
        }
        else
        {
            if (index == -1)
            {
                if (n == 8)
                    index++;
                else
                {
                    at_sender->play_local_melody("3,2,1,1,1"); // Fail
                    mode = NOTHING;
                    return true;
                }
            }
            else
            {
                buffer[index] = n;
                index++;
                if (index == 10)
                {
                    EEPROM.get(0, eeprom_layout);
                    for (int i = 0; i < 10; i++)
                    {
                        eeprom_layout.contacts[n][i] = buffer[i];
                    }
                    EEPROM.put(0, eeprom_layout); 
                    at_sender->play_local_melody("1,1,1,2,3"); // Success
                    mode = NOTHING;
                    return true;
                }
            }
        }
        break;
    case CALLING_CONTACT:
        EEPROM.get(0, eeprom_layout);
        for (int i = 0; i < 10; i++)
        {
            buffer[i] = eeprom_layout.contacts[n][i];
        }
        call();
        // return true;
        break;
    case READING_CONTACT:
        EEPROM.get(0, eeprom_layout);
        for (int i = 0; i < 10; i++)
        {
            at_sender->play_local_sound(String(eeprom_layout.contacts[n][i]) + ".amr");
        }
        return true;
        break;
    case CALLING_NUMBER:
        buffer[index] = n;
        index++;
        if (index == 10)
        {            
            call();
            mode = NOTHING;
            // return true;
        }
        break;
    case CALLING:
        at_sender->play_remote_sound("aboba.amr");
        break;
    default:
        break;
    }
    return false;
}
