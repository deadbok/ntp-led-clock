 /**
 * NTP LED Clock.
 * 
 * main.cpp
 *
 * Main program file.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */

#include <ap_secret.h>
#include <hardware.h>
#include <TM1650.h>
#include <version.h>

TM1650 display;

void setup()
{
    Serial.begin(115200);
    Serial.printf("NTP LED Clock %s.", VERSION);

    Wire.begin(TM1650_SDA, TM1650_SCL);

    display.init();
}

void loop()
{
}
