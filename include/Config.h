/**
 * NTP LED Clock.
 * 
 * config.h
 *
 * Class to hold and manage config values the clock.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#define CONFIGSTORAGE_DEBUG true // show debug messages for ConfigStorage

#include <ArduinoJson.h>
#include <WString.h>
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

        void setTime(String time);
        String getTime();

        void setBrightness(unsigned char brightness);
        unsigned char getBrightness();

        void setSSID(String SSID);
        String getSSID();

        void setHostname(String hostname);
        String getHostname();

        void setNTPenable(bool NTP);
        bool getNTPenable();    

        ~Config();
    private:
        //Version of the configuration data in the class.
        //Major version bump: Breaking changes
        const unsigned char vmajor = 0;
        //Minor version bump: Backward compatible changes.
        unsigned char       vminor = 1;

        //Actual confiuration data.
        String              NTPServer;
        String	            timeZone;
        String			    time;
		unsigned char       brightness;
        String			    SSID;
		String              hostname;
        bool                NTP;

        //Has the configuration changed?
        bool                changed;
        //Clock object.
        NRClock&            clock;
        //JSON document that for the configuration.
        JsonDocument        configDoc;
        
        void updateJSON();
        void updateFromJSON();
        //Read/Write configuration to flash.
        bool readJSON(String filename);
        bool writeJSON(String filename);
};

#endif
