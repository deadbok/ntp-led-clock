#include "Config.h"
#include "hardware.h"
#include <LittleFS.h>

Config::Config(NRClock& nrclock) : NTPServer(""), timeZone(""), time(""),
                                   brightness(4), SSID(""),
                                   hostname("clock" + chipID()),
                                   changed(false), clock(nrclock),
                                   configDoc()
{
   // this->readJSON(config_fn);
}

void Config::setNTPServer(String server)
{
    this->changed = true;
    this->NTPServer = server;
    this->writeJSON();
}

String Config::getNTPServer()
{
    return(this->NTPServer);
}

void Config::setTimeZone(String TZ)
{
    this->changed = true;
    this->timeZone = TZ;
    this->writeJSON();
}

String Config::getTimeZone()
{
    return(this->timeZone);
}

void Config::setTime(String time)
{
    this->changed = true;
    this->time = time;
    this->writeJSON();
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
    this->writeJSON();
}

unsigned char Config::getBrightness()
{
    return(this->brightness);
}

void Config::setSSID(String SSID)
{
    this->changed = true;
    this->SSID = SSID;
    this->writeJSON();    
}

String Config::getSSID()
{
    return(this->SSID);
}

void Config::setHostname(String hostname)
{
    this->changed = true;
    this->hostname = hostname;
    this->writeJSON();    
}

String Config::getHostname()
{
    return(this->hostname);
}

void Config::setNTPenable(bool NTP)
{
    this->changed = true;
    this->NTP = NTP;
    this->writeJSON();    
}

bool Config::getNTPenable()
{
    return(this->NTP);
}

void Config::updateJSON()
{
    this->configDoc.clear();

    this->configDoc["ma_ver"] = this->vmajor;
    this->configDoc["mi_ver"] = this->vminor;

    this->configDoc["ntp_srv"] = this->NTPServer;
    this->configDoc["tz"] = this->timeZone;

    this->configDoc["brightness"] = this->brightness;
    this->configDoc["ssid"] = this->SSID;
    this->configDoc["host"] = this->hostname;
    this->configDoc["ntp_en"] = this->NTP;

    this->changed = false;
}

void Config::updateFromJSON()
{
    if (this->configDoc["ma_ver"].as<unsigned char>() > this->vmajor)
    {
        Serial.println("Configuration data newer than parser.");
        return;
    }
    
    this->NTPServer = this->configDoc["ntp_srv"].as<String>();
    this->timeZone = this->configDoc["tz"].as<String>();

    this->brightness = this->configDoc["brightness"];
    this->SSID = this->configDoc["ssid"].as<String>();
    this->hostname = this->configDoc["host"].as<String>();
    this->NTP = this->configDoc["ntp_en"].as<bool>();

    this->changed = false;

}

bool Config::readJSON()
{
    Serial.printf("Reading configuration file: %s\r\n", config_fn.c_str());

    File file = LittleFS.open(config_fn, FILE_READ);
    if (!file)
    {
        Serial.println("Could not open file.");
        return(false);
    }
    
    DeserializationError error = deserializeJson(configDoc, file);
    if (error)
    {
        Serial.println(F("Failed to read file."));
        return(false);
    }

    file.close();

    Serial.println("Read configuration.");
#ifdef DEBUG    
    serializeJsonPretty(configDoc, Serial);
    Serial.println();
#endif

    this->updateFromJSON();
    return(true);
}

bool Config::writeJSON()
{
    if (changed)
    {
        updateJSON();

        Serial.printf("Writing configuration file: %s\r\n", config_fn.c_str());

        File file = LittleFS.open(config_fn, FILE_WRITE);
        if (!file)
        {
            Serial.println("Could not open file");
            return(false);
        }
        
        if (serializeJson(configDoc, file) == 0)
        {
            Serial.println("Could not save file.");
            return(false);
        }

        Serial.println("Saving configuration.");
#ifdef DEBUG
        serializeJsonPretty(configDoc, Serial);
        Serial.println();
#endif

        file.close();
    }
    else
    {
        Serial.println("No configuration changes. Nothing to be written.");
    }
    return(true);
}

Config::~Config()
{
}