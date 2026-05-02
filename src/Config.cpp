#include "Config.h"
#include "hardware.h"
#include <LittleFS.h>
#include <WiFi.h>

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
#ifdef DEBUG
    Serial.printf("Setting NTP server: %s\n", server.c_str());
#endif
    this->changed = true;
    this->NTPServer = server;
}

String Config::getNTPServer()
{
    return(this->NTPServer);
}

void Config::setTimeZoneIndex(unsigned short index)
{
#ifdef DEBUG
    Serial.printf("Setting time zone index: %d\n", index);
#endif
    this->changed = true;
    this->timeZoneIndex = index;
}

unsigned short Config::getTimeZoneIndex()
{
    return(this->timeZoneIndex);
}

void Config::setTimeZone(String TZ)
{
#ifdef DEBUG
    Serial.printf("Setting time zone: %s\n", TZ.c_str());
#endif
    this->changed = true;
    this->timeZone = TZ;
}

String Config::getTimeZone()
{
    return(this->timeZone);
}

void Config::setTime(String time)
{
#ifdef DEBUG
    Serial.printf("Setting time: %s\n", time.c_str());
#endif
    this->changed = true;
    this->time = time;
}

String Config::getTime()
{
    return(this->time);
}

void Config::setBrightness(unsigned char brightness)
{
#ifdef DEBUG
    Serial.printf("Setting brightness %d\n", brightness);
#endif
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
#ifdef DEBUG
    Serial.printf("Setting SSID: %s\n", SSID.c_str());
#endif
    this->changed = true;
    this->SSID = SSID;
}

String Config::getSSID()
{
    return(this->SSID);
}

//TODO: For static IP and stuff this getter/setter will need work.
void Config::setIP(String ip)
{
#ifdef DEBUG
    Serial.printf("Setting IP: %s\n", ip.c_str());
#endif
    this->changed = true;
    this->IP = ip;
}

String Config::getIP()
{
    return(WiFi.localIP().toString());
}

void Config::setHostname(String hostname)
{
#ifdef DEBUG
    Serial.printf("Setting host name: %s\n", hostname.c_str());
#endif
    this->changed = true;
    this->hostname = hostname;
}

String Config::getHostname()
{
    return(this->hostname);
}

void Config::setNTPenable(bool NTP)
{
#ifdef DEBUG
    Serial.printf("Setting NTP enable: %d\n", NTP);
#endif
    this->changed = true;
    this->NTP = NTP;
}

bool Config::getNTPenable()
{
    return(this->NTP);
}

// Return the config data as a serialised string
String Config::getJSON()
{
    size_t  ret;
    String  serialJSON;

    if (changed)
    {
        updateJSON();
    }

    ret = serializeJson(configDoc, serialJSON);
    if (ret == 0)
    {
        Serial.println(F("Serialised document is empty."));
        return("{}");
    }

#ifdef DEBUG
    Serial.println(serialJSON);
#endif

    return(serialJSON);
}

int Config::setFromJSON(String json)
{
#ifdef DEBUG
    Serial.println(F("Updating configuration from JSON string:"));
    Serial.println(json);
#endif

    DeserializationError error = deserializeJson(configDoc, json);
    if (error)
    {
        Serial.println(F("Failed to deserialise the data."));
        return(-1);
    }

#ifdef DEBUG
    Serial.println(F("JSON data:"));  
    serializeJsonPretty(configDoc, Serial);
    Serial.println();
#endif

    this->updateFromJSON();
    return(0);   
}

// Update the JSON document from values in the config object
void Config::updateJSON()
{
    this->configDoc.clear();

    this->configDoc["ma_ver"] = this->vmajor;
    this->configDoc["mi_ver"] = this->vminor;

    this->configDoc["ntp_server"] = this->NTPServer;
    this->configDoc["tz_value"] = this->timeZone;
    this->configDoc["tz_index"] = this->timeZoneIndex;

    this->configDoc["brightness"] = this->brightness;
    this->configDoc["ssid"] = this->SSID;
    this->configDoc["ip"] = this->IP;
    this->configDoc["hostname"] = this->hostname;
    this->configDoc["ntp_en"] = this->NTP;

    this->changed = false;
}

// Update the values in the config object from the JSON document
void Config::updateFromJSON()
{
    if (this->configDoc["ma_ver"].as<unsigned char>() > this->vmajor)
    {
        Serial.println("Configuration data newer than parser.");
        return;
    }
    
    setNTPServer(this->configDoc["ntp_server"].as<String>());
    setNTPenable(this->configDoc["ntp_en"].as<bool>());

    setTimeZone(this->configDoc["tz_value"].as<String>());
    setTimeZoneIndex(this->configDoc["tz_index"]);

    setBrightness(configDoc["brightness"]);
    
    setSSID(this->configDoc["ssid"].as<String>());
    setIP(this->configDoc["ip"].as<String>());
    setHostname(configDoc["hostname"].as<String>());
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