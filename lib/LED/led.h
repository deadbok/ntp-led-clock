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
