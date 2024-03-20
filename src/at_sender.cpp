#include "at_sender.hpp"

AtSender::AtSender()
{
    Serial.begin(9600);
    this->volume_level = 100;
    pinMode(LED_BUILTIN, OUTPUT);
    
    digitalWrite(LED_BUILTIN, HIGH);
}

AtSender::AtSender(int volume_level)
{
    Serial.begin(9600);
    this->volume_level = volume_level;
    pinMode(LED_BUILTIN, OUTPUT);
    
    digitalWrite(LED_BUILTIN, HIGH);
}

void AtSender::call(String s)
{
    Serial.println("ATD" + s + ";");
}

void AtSender::set_volume_level(int volume_level)
{
    this->volume_level = volume_level;
}

void AtSender::play_local_sound(String s)
{
    Serial.println("AT+CREC=4,C:\\" + s + ",0," + volume_level);
    wait_playing();
}

void AtSender::play_local_melody(String s)
{
    Serial.println("AT+CLDTMF=5,\"" + s + "\"");
}

void AtSender::wait_playing()
{
    digitalWrite(LED_BUILTIN, LOW);
    while (1)
    {
        String s = Serial.readString();
        if (s.length() < 8)
            continue;
        for (int i = 0; i < 8; i++)
            if (s[i] != "+CREC: 0"[i])
                continue;
        break;
    }
    digitalWrite(LED_BUILTIN, HIGH);
}
