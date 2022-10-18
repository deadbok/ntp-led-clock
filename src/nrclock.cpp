/**
 * NTP LED Clock.
 * 
 * nrclock.c
 *
 * Clock class implementation. Takes care of all clock functions both RTC & NTP.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */

#include "nrclock.h"
#include <TZ.h>
#include <time.h>
#include <sys/time.h>
#include "hardware.h"

NRClock::NRClock() : display(), RTCWire(DS1302_IO, DS1302_SCLK, DS1302_CE),
           		     RTC(RTCWire)
{}

void NRClock::init()
{
	last_minute = 0;

    // Initialise the 7-segment display
    Wire.begin(TM1650_SDA, TM1650_SCL);

    display.init();
    display.displayOn();

    //Look alive
    for (int i = 0; i < 4; i++)
    {
        display.setDot(i, true);
    }

    //Initialise the RTC.
    RTC.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (RTC.GetIsWriteProtected())
    {
        Serial.println(F("Enabling RTC writing"));
        RTC.SetIsWriteProtected(false);
    }

    if (!RTC.IsDateTimeValid()) 
    {
        Serial.println(F("RTC time is not valid, setting default"));
        RTC.SetDateTime(compiled);
    }

    if (!RTC.GetIsRunning())
    {
        Serial.println(F("Starting RTC"));
        RTC.SetIsRunning(true);
    }

    //Set up NTP
    Serial.println(F("Setting up NTP client"));
	configTime(0, 0, NTP_SERVERS);

    //Set time zone for Denmark
	setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
	tzset();
}

void NRClock::update()
{
    struct tm       *time_info;
    RtcDateTime     rtc_now = RTC.GetDateTime();
    time_t          now;
    char digits[] = "0000";

    if (last_minute != rtc_now.Minute())
    {
        last_minute = rtc_now.Minute();
    
        //Get UTC from RTC
        now = rtc_now.Epoch32Time();
        //Convert to local time
        time_info = localtime(&now);

        //Get time into a 4 charater string for the display  
        digits[0] = '0' + (time_info->tm_hour / 10);
        digits[1] = '0' + (time_info->tm_hour % 10);
        digits[2] = '0' + (time_info->tm_min / 10);
        digits[3] = '0' + (time_info->tm_min % 10);
    
        Serial.println(digits);
        display.displayString(digits);
    }
}

void NRClock::dots(bool state)
{
	display.setDot(1, state);
}

void NRClock::setDateTime(RtcDateTime datetime)
{
	RTC.SetDateTime(datetime);
} 

void NRClock::setBrightess(unsigned char val)
{
    display.setBrightness(val);
}

NRClock::~NRClock()
{}