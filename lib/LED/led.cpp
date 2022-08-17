 /**
 * NTP LED Clock.
 * 
 * led.cpp
 *
 * Control of the LED. 
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <led.h>

//Constructor
LED::LED(unsigned char led_pin) : pin(led_pin)
{
}

//Initialise the LED to off
void LED::init()
{
    pinMode(this->pin, OUTPUT);
    this->off();
}

//Turn the LED on
void LED::on()
{
    digitalWrite(this->pin, 0);
}

//Turn the LED off
void LED::off()
{
    digitalWrite(this->pin, 1);
}

//Toggle the LED
void LED::toggle()
{
    digitalWrite(this->pin, !this->getState());
}

//Get the state of the LED
bool LED::getState()
{
    return(digitalRead(pin));
}