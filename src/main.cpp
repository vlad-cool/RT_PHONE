#include <Arduino.h>
#include <SoftwareSerial.h>

#include "at_sender.hpp"
#include "simon_says.hpp"
#include "phone.hpp"
#include "settings.hpp"

#define PULSE_PIN 2
#define ENTERING_PIN A5
#define RESET_PIN 3

AtSender at_sender(100);
Simon simon_says(&at_sender);
Phone phone(&at_sender);
Settings settings(&at_sender);

int i1 = 0;

int mode = -1;
int digit = 0;
unsigned long pulse_interrupt_time = 0;

unsigned long last_end_interrupt_time = 0;

int prev_A5 = 0;

char buf[11];
int i = 0;
int j = 0;

void pulse_interrupt();
void enter_digit();
void reset_interrupt();

void setup()
{
    pinMode(PULSE_PIN, INPUT_PULLUP);
    pinMode(ENTERING_PIN, INPUT_PULLUP);
    pinMode(RESET_PIN, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(PULSE_PIN), pulse_interrupt, HIGH);
    attachInterrupt(digitalPinToInterrupt(RESET_PIN), reset_interrupt, HIGH);
}

void loop()
{
    if (digit != 0 and digitalRead(ENTERING_PIN))
    {
        enter_digit();
    }
}

void pulse_interrupt()
{
    if (micros() - pulse_interrupt_time < 50000)
        return;
    digit++;
    pulse_interrupt_time = micros();
}

void reset_interrupt()
{
    switch (mode)
    {
    case 1:
    case 2:
    case 3:
    case 8:
        phone.stop();
        break;
    case 9:
        simon_says.stop();
        break;
    case 0:
        settings.stop();
        break;
    }
    mode = -1;
    digit = 0;
}

void enter_digit()
{
    digit = (digit + 9) % 10;
    at_sender.play_local_sound(String(digit) + ".amr");
    switch (mode)
    {
    case -1:
        switch (digit)
        {
        case 1:
            phone.call_contact();
            mode = 1;
            break;
        case 2:
            phone.add_contact();
            mode = 2;
            break;
        case 3:
            phone.read_contact();
            mode = 3;
            break;
        case 8:
            phone.call_number();
            mode = 8;
            break;
        case 9:
            simon_says.start_game();
            mode = 9;
            break;
        case 0:
            mode = 0;
            break;
        default:
            break;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 8:
        if (phone.enter_number(digit))
        {
            mode = -1;
        }
        break;
    case 9:
        simon_says.enter_number(digit);
        break;
    case 0:
        settings.enter_number(digit);
    default:
        mode = -1;
        break;
    }
    digit = 0;
}
