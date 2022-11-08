#include <Esp.h>
#include "Config.h"

Config::Config(NRClock& nrclock) : NTPServer(""), timeZone(""), time(""),
                                   brightness(4), SSID(""),
                                   hostname("clock" + ESP.getChipId()),
                                   changed(false), clock(nrclock)
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

Config::~Config()
{
}