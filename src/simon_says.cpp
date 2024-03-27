#include "simon_says.hpp"

Simon::Simon(AtSender *at_sender)
{
    randomSeed(analogRead(A6) + analogRead(A7) + analogRead(A2) + analogRead(A3) + analogRead(A4));
    this->at_sender = at_sender;
}

void Simon::stop()
{

}

void Simon::start_game()
{
    index_1 = 0;
    index_2 = 0;
    at_sender->play_local_sound("game_started.amr");
    next_number();
}

void Simon::enter_number(int n)
{
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
    int number = random(0, 10 - index_1);
    for (int i = 0; i < index_1; i++)
    {
        if (number == buffer[i])
        {
            number++;
            i = 0;
        }
    }
    number = random(0, 10);
    buffer[index_1] = number;
    index_1++;
    index_2 = 0;

    for (int i = 0; i < index_1; i++)
    {
        at_sender->play_local_sound(String(buffer[i]) + ".amr");
    }

    at_sender->play_local_melody("0"); // Ready to enter numbers
}

void Simon::win()
{
    if (random(0, 2))
    {
        at_sender->play_local_sound("won_1.amr");
    }
    else
    {
        at_sender->play_local_sound("won_2.amr");
    }
    start_game();
}

void Simon::loose()
{
    at_sender->play_local_sound("lose.amr");
    start_game();
}
