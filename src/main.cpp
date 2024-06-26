#include <Arduino.h>
#include <SoftwareSerial.h>

#include "anti_rattle.hpp"
#include "at_sender.hpp"
#include "simon_says.hpp"
#include "phone.hpp"
#include "settings.hpp"

#define PULSE_PIN 2
#define ENTERING_PIN A5
#define CANCEL_PIN 3
#define RING_IN_PIN 7
#define RING_OUT_PIN 12

#define BUZZER

AtSender at_sender(100);
Simon simon_says(&at_sender);
Phone phone(&at_sender);
Settings settings(&at_sender);

volatile int mode = -1;
volatile int digit = 0;

void pulse_interrupt();
void enter_digit();
void reset_interrupt();

void setup()
{
    pinMode(PULSE_PIN, INPUT_PULLUP);
    pinMode(ENTERING_PIN, INPUT_PULLUP);
    pinMode(CANCEL_PIN, INPUT_PULLUP);
    pinMode(RING_IN_PIN, INPUT);
    pinMode(RING_OUT_PIN, OUTPUT);

    settings = Settings(&at_sender);
    at_sender = AtSender(settings.get_volume_level());
    simon_says = Simon(&at_sender);
    phone = Phone(&at_sender);

    attachInterrupt(digitalPinToInterrupt(PULSE_PIN), pulse_interrupt, HIGH);
    attachInterrupt(digitalPinToInterrupt(CANCEL_PIN), reset_interrupt, CHANGE);
}

void loop()
{
    at_sender.set_volume_level(settings.get_volume_level());
    if (digitalRead(ENTERING_PIN))
    {
        if (digit > 1)
        {
            enter_digit();
        }
        else
        {
            digit = 0;
        }
    }
    if (digitalRead(RING_IN_PIN) == 0)
    {
        #ifdef BUZZER
            if ((millis() % 4000) > 2000)
            {
                tone(RING_OUT_PIN, 2000);
            }
            else
            {
                noTone(RING_OUT_PIN); 
            }
        #else
            digitalWrite(RING_OUT_PIN, (millis() % 4000) > 2000);
        #endif
    }
    else
    {
        #ifdef BUZZER
            noTone(RING_OUT_PIN); 
        #else
            digitalWrite(RING_OUT_PIN, 0);
        #endif
    }
}

void pulse_interrupt()
{
    static AntiRattle anti_rattle = AntiRattle();
    if (digitalRead(ENTERING_PIN))
        return;
    if (anti_rattle.test())
    {
        digit++;
    }
    // Serial.println(digit);
}

void reset_interrupt()
{
    static AntiRattle anti_rattle = AntiRattle();
    // if (anti_rattle.test())
    {
        if (digitalRead(CANCEL_PIN) == 0)
        {
            switch (mode)
            {
            case 1:
            case 2:
            case 3:
            case 8:
            case 10:
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
        else
        {
            if (digitalRead(RING_IN_PIN) == 0)
            {
                phone.accept_call();
                mode = 10;
            }
            else
            {
                // at_sender.play_local_melody("1,4,7,4,1,4,7"); // Welcome
            }
        }
    }
}

void enter_digit()
{
    int processed_digit = (digit + 9) % 10;
    digit = 0;
    // Serial.println("Num " + String(processed_digit));
    if (settings.get_echo())
    {
        at_sender.play_local_sound(String(processed_digit) + ".amr");
    }
    switch (mode)
    {
    case -1:
        switch (processed_digit)
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
        case 7:
            at_sender.play_local_sound("aboba.amr");
            break;
        case 8:
            phone.call_number();
            mode = 8;
            break;
        case 9:
            at_sender.play_local_sound("denchik_says.amr");
            simon_says.start_game();
            mode = 9;
            break;
        case 0:
            mode = 0;
            at_sender.play_local_sound("settings.amr");
            break;
        default:
            break;
        }
        break;
    case 1:
    case 2:
    case 3:
    case 8:
        if (phone.enter_number(processed_digit))
        {
            mode = -1;
        }
        break;
    case 9:
        simon_says.enter_number(processed_digit);
        break;
    case 0:
        settings.enter_number(processed_digit);
        break;
    default:
        mode = -1;
        break;
    }
}
