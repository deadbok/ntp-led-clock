/**
 * NTP LED Clock.
 * 
 * config.h
 *
 * Class to hold and manage config values the clock.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <WString.h>
#include "ArduinoJson.h"
#include "NRClock.h"

#ifndef config_h
#define config_h

class Config
{
    public:
        Config(NRClock& nrclock);

        void setNTPServer(String server);
        String getNTPServer();

        void setTimeZone(String TZ);
        String getTimeZone();

        void setRegion(String region);
        String getRegion();        

        void setTime(String time);
        String getTime();

        void setBrightness(unsigned char brightness);
        unsigned char getBrightness();

        void setSSID(String SSID);
        String getSSID();

        void setHostname(String hostname);
        String getHostname();

        String& getJSON(String var_name);

        ~Config();
    private:
        //Version of the configuration data in the class.
        const unsigned char vmajor = 0;
        const unsigned char vminor = 1;

        //Actual confiuration data.
        String              NTPServer;
        String	            timeZone;
        String              region;
        String			    time;
		unsigned char       brightness;
        String			    SSID;
		String              hostname;

        //Has the configuration changed?
        bool                changed;
        //Clock object.
        NRClock&            clock;
        //JSON document for config.
        String              JSONConfig;
};

#endif
