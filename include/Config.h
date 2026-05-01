/**
 * NTP LED Clock.
 * 
 * config.h
 *
 * Class to hold and manage config values the clock.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
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

        void setTimeZoneIndex(unsigned short index);
        unsigned short getTimeZoneIndex();

        void setTimeZone(String TZ);
        String getTimeZone();

        void setTime(String time);
        String getTime();

        void setBrightness(unsigned char brightness);
        unsigned char getBrightness();

        void setSSID(String SSID);
        String getSSID();

        void setIP(String ip);
        String getIP();

        void setHostname(String hostname);
        String getHostname();

        void setNTPenable(bool NTP);
        bool getNTPenable();
        
        // Get JSON string of config data.
        String getJSON();

        // Set config data from JSON string.
        int setFromJSON(String json);

        //Read/Write configuration to flash.
        bool readJSON();
        bool writeJSON();

        ~Config();
    private:
        //Version of the configuration data in the class.
        //Major version bump: Breaking changes
        const unsigned char vmajor = 0;
        //Minor version bump: Backward compatible changes.
        const unsigned char vminor = 2;

        //Actual confiuration data.
        String              NTPServer;
        String	            timeZone;
        unsigned short      timeZoneIndex;
        String			    time;
		unsigned char       brightness;
        String			    SSID;
        String              IP;
		String              hostname;
        bool                NTP;

        //Has the configuration changed?
        bool                changed;
        //Clock object.
        NRClock&            clock;
        //JSON document that for the configuration.
        JsonDocument        configDoc;
        //The filename of the configuration file
        const String        config_fn = "/config.json";
        
        void updateJSON();
        void updateFromJSON();
};

#endif
