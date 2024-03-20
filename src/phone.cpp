#include "phone.hpp"

Phone::Phone(AtSender *at_sender)
{
    mode = NOTHING;
    contact_id = -1;
    index = 0;
    this->at_sender = at_sender;
}

void Phone::call()
{
    String s = "000000000";
    for (int i = 0; i < 10; i++)
    {
        s[i] += buffer[i];
    }
    at_sender->call(s);
}

void Phone::add_contact()
{
    mode = ADDING_CONTACT_ID;
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
            at_sender->play_local_melody("3,2,1,1,1"); // Ошибка
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
            at_sender->play_local_melody("3,2,1,1,1"); // Ошибка
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
                    at_sender->play_local_melody("3,2,1,1,1"); // Ошибка
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
                    EEPROM.put(contact_id * 10, buffer); 
                    at_sender->play_local_melody("1,1,1,2,3"); // Успешно
                    mode = NOTHING;
                    return true;
                }
            }
        }
        break;
    case CALLING_CONTACT:
        EEPROM.get(n * 10, buffer);
        call();
        return true;
        break;
    case CALLING_NUMBER:
        buffer[index] = n;
        index++;
        if (index == 10)
        {            
            call();
            mode = NOTHING;
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}
