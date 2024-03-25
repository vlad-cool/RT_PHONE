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
    Serial.println("ATD+7" + s + ";");
}

void AtSender::drop_call()
{
    Serial.println("ATH");
}

void AtSender::accept_call()
{
    Serial.println("ATA");
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

void AtSender::play_remote_sound(String s)
{
    Serial.println("AT+CREC=4,C:\\" + s + ",1," + volume_level);
    wait_playing();
}

void AtSender::play_local_melody(String s)
{
    Serial.println("AT+CLDTMF=5,\"" + s + "\"");
}

void AtSender::wait_playing()
{
    digitalWrite(LED_BUILTIN, LOW);
    bool flag = true;
    while (flag)
    {
        flag = false;
        char c;
        while (Serial.read() != '+') { }
        delay(100);
        for (int i = 1; i < 8; i++)
        {
            c = Serial.read();
            if (c != "+CREC: 0"[i])
            {
                flag = true;
                break;
            }
        }
    }
    digitalWrite(LED_BUILTIN, HIGH);
}
