#ifndef SIMON_SAYS
#define SIMON_SAYS

#include "at_sender.hpp"

#define SIMON_BUFSIZE 10

class Simon {
private:
    int index_1;
    int index_2;
    int buffer[SIMON_BUFSIZE];
    AtSender *at_sender;
public:
    Simon(AtSender *at_sender);
    void start_game();
    void enter_number(int n);
    void next_number();
    void win();
    void loose();
};

#endif