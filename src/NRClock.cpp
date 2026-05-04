/**
 * NTP LED Clock.
 * 
 * nrclock.c
 *
 * Clock class implementation. Takes care of all clock functions both RTC & NTP.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */

#include "NRClock.h"
//#include <TZ.h>
#include <time.h>
#include <sys/time.h>
#include "hardware.h"

NRClock::NRClock() : display(TM1637_SCL, TM1637_SDA), RTC(Wire), last_minute(0)
{}

void NRClock::init()
{
    Serial.printf("Initialising clock.\n");
    // Initialise the 7-segment display
    display.init();
    display.colonOff();
    display.display("init");

    //Look alive
    display.colonOn();

    //Initialise the RTC.
    RTC.Begin(DS3231_SDA, DS3231_SCL);

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    Serial.printf("Disabling RTC 32 kHz output\n");
    RTC.Enable32kHzPin(false);

    if (!RTC.IsDateTimeValid()) 
    {
        Serial.print("RTC time is not valid, setting default\n");
        RTC.SetDateTime(compiled);
    }

    if (!RTC.GetIsRunning())
    {
        Serial.printf("Starting RTC");
        RTC.SetIsRunning(true);
    }

    //Set up NTP
    Serial.printf("Setting up NTP client\n");
	configTime(0, 0, NTP_SERVERS);

    //Set time zone for Denmark
	setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
	tzset();

	Serial.printf("Clock initialisation done\n");
}

void NRClock::update()
{
    struct tm       *time_info;
    RtcDateTime     rtc_now = RTC.GetDateTime();
//    time_t          now;
    char			digits[5];

    if (last_minute != rtc_now.Minute())
    {
        last_minute = rtc_now.Minute();
    
        //Get UTC from RTC
        time_t now = rtc_now.Unix32Time();
        //Convert to local time
        time_info = localtime(&now);

        //Get time into a 4 charater string for the display  
      /*  digits[0] = '0' + (time_info->tm_hour / 10);
        digits[1] = '0' + (time_info->tm_hour % 10);
        digits[2] = '0' + (time_info->tm_min / 10);
        digits[3] = '0' + (time_info->tm_min % 10);*/
    
        sprintf(digits, "%02d%02d", time_info->tm_hour, time_info->tm_min);
#ifdef DEBUG
        Serial.printf("Time updated: %s\n", digits);
#endif

        display.display(String(digits));
    }
}

void NRClock::dots(bool state)
{
    if (state)
    {
        display.colonOn();
    }
    else
    {
        display.colonOff();
    }
}

void NRClock::setDateTime(RtcDateTime datetime)
{
	RTC.SetDateTime(datetime);
	update();
} 

unsigned char NRClock::getBrightness()
{
    return(display.getBrightness());
}

void NRClock::setBrightness(unsigned char val)
{
    display.setBrightness(val);
}

NRClock::~NRClock()
{}