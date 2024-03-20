#include "simon_says.hpp"

Simon::Simon(AtSender *at_sender)
{
    randomSeed(analogRead(A0));
    this->at_sender = at_sender;
}

void Simon::stop()
{
    
}

void Simon::start_game()
{
    index_1 = 0;
    index_2 = 0;
    at_sender->play_local_melody("3,3,3,2,2,2,1,1,1");
    delay(6000);
    next_number();
}

void Simon::enter_number(int n)
{
    at_sender->play_local_melody("9,4");
    delay(2000);

    if (buffer[index_2] == n)
    {
        index_2++;
        if (index_2 == SIMON_BUFSIZE)
        {
            win();
        }
        else if (index_2 == index_1)
        {
            next_number();
        }
    }
    else
    {
        loose();
    }
}

void Simon::next_number()
{
    buffer[index_1] = random(0, 10);
    index_1++;
    index_2 = 0;

    for (int i = 0; i <= index_1; i++)
    {
        at_sender->play_local_sound(String(buffer[i]) + ".amr");
    }
}

void Simon::win()
{
    at_sender->play_local_melody("3,2,1,1,2,3,3,2,1");
    delay(8000);
    start_game();
}

void Simon::loose()
{
    at_sender->play_local_melody("1,2,3,1,2,1");
    delay(4000);
    start_game();
}
