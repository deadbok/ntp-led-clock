#include <Esp.h>
#include "debug.h"
#include "Config.h"

Config::Config(NRClock& nrclock) :  NTPServer(""), timeZone(""), time(""),
                                    brightness(4), SSID(""),
                                    hostname("clock" + ESP.getChipId()),
                                    changed(false), clock(nrclock),
                                    JSONConfig()
{
}

void Config::setNTPServer(String server)
{
    this->NTPServer = server;
}

String Config::getNTPServer()
{
    return(this->NTPServer);
}

void Config::setTimeZone(String TZ)
{
    this->timeZone = TZ;
}

String Config::getTimeZone()
{
    return(this->timeZone);
}

void Config::setRegion(String region)
{
    this->region = region;
}

String Config::getRegion()
{
    return(this->region);
}

void Config::setTime(String time)
{
    this->time = time;
}

String Config::getTime()
{
    return(this->time);
}

void Config::setBrightness(unsigned char brightness)
{
    this->brightness = brightness;
    this->changed = true;
    this->clock.setBrightness(brightness);
}

unsigned char Config::getBrightness()
{
    return(this->brightness);
}

void Config::setSSID(String SSID)
{
    this->SSID = SSID;
}

String Config::getSSID()
{
    return(this->SSID);
}

void Config::setHostname(String hostname)
{
    this->hostname = hostname;
}

String Config::getHostname()
{
    return(this->hostname);
}

/*
 * {
 *  "NTPServer": "dk.pool.ntp.org",
 *  "TZ": "CET-1CEST,M3.5.0,M10.5.0/3",
 *  "region": "America/Argentina/Buenos_Aires",
 *  "time": "00:00:00",
 *  "brightness": 4,
 *  "SSID": "Ir6pGk2ccdqloaCABJ8pV7kkh7UTkkIi",
 *  "hostname": "clock1947093"
 * }
 */
String& Config::getJSON(String var_name)
{
    DynamicJsonDocument JSONConfigDoc(400);

    JSONConfigDoc["NTPServer"] = this->getNTPServer();
    JSONConfigDoc["TZ"] = this->getTimeZone();
    JSONConfigDoc["region"] = this->getRegion();
    JSONConfigDoc["time"] = this->getTime();
    JSONConfigDoc["brightness"] = this->getBrightness();
    JSONConfigDoc["SSID"] = this->getSSID();
    JSONConfigDoc["hostname"] =this->getHostname();

    if ((var_name == "") || (var_name == "all"))
    {
        //Whole object
        serializeJson(JSONConfigDoc, JSONConfig);
    }
    else
    {
        char    i = 0;
        char    ii = 0;

        //Split by comma if needed.
        ii = var_name.indexOf(",");
        if (ii == -1)
            //No comma just one entry
            serializeJson(JSONConfigDoc[var_name], JSONConfig);
        else
        {
            //Multiply entries
            JSONConfigDoc.clear();
         
            while(ii > -1)
            {
                String key = var_name.substring(i, ii);

                switch (key[0])
                {
                    case 'N':
                        JSONConfigDoc["NTPServer"] = this->getNTPServer();
                        break;
                    case 'T':
                        JSONConfigDoc["TZ"] = this->getTimeZone();
                        break;
                    case 'r':
                        JSONConfigDoc["region"] = this->getRegion();
                        break;                        
                    case 't':
                        JSONConfigDoc["time"] = this->getTime();
                        break;
                    case 'b':
                        JSONConfigDoc["brightness"] = this->getBrightness();
                        break;
                    case 'S':
                        JSONConfigDoc["SSID"] = this->getSSID();
                        break;
                    case 'h':
                        JSONConfigDoc["hostname"] = this->getHostname();
                        break;
                    default:
                        warn("\"%s\" not found in config\n", key.c_str());
                        break;
                }

                //Next entry
                i = ii;
                ii = var_name.indexOf(",", i);
            }
            serializeJson(JSONConfigDoc, JSONConfig);
        }
    }
    Serial.println(JSONConfig);

    return(JSONConfig);
}

Config::~Config()
{
}