 /**
 * NTP LED Clock.
 * 
 * led.h
 *
 * Control of the LED. 
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <Arduino.h>

#ifndef _LED_H_
#define _LED_H_

class LED
{
public:
	LED(unsigned char led_pin);

    void init();
    void on();
    void off();
    void toggle();
    bool getState();
private:
	unsigned char pin;
};

#endif