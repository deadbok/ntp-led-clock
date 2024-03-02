#include "RTWebServer.h"
#include <LittleFS.h>

RTWebServer::RTWebServer(Config& config) : server(80), config(config)
{}

void RTWebServer::start()
{
    // Serve files needed for the index page.
    server.serveStatic("/", LittleFS, "index.html");
    server.serveStatic("/css/normalize.css", LittleFS, "/css/normalize.css");
    server.serveStatic("/css/custom.css", LittleFS, "/css/custom.css");
    server.serveStatic("/favicon.ico", LittleFS, "/favicon.ico");

    // Endpoints for configuration
    server.on("/ntptz", std::bind(&RTWebServer::handleNTPTZ, this));
    server.on("/time", std::bind(&RTWebServer::handleTime, this));    
    server.on("/brightness", std::bind(&RTWebServer::handleBrightness, this));
    server.on("/net", std::bind(&RTWebServer::handleNet, this));  

    //Not found (404) page.
    server.onNotFound(std::bind(&RTWebServer::onNotFound, this));

    //Start the web server
    server.begin();
}

void RTWebServer::handleNTPTZ()
{
    if (server.method() == HTTP_GET)
    {
        Serial.println("GET");
        String NTPTZ = this->config.getJSON("NTPServer, TZ, region");
        server.send(200, "text/plain", NTPTZ);
    }
    else if (server.method() == HTTP_POST)
    {
        Serial.println("POST");
        String json = server.arg("plain");
        Serial.println(json);
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(501, "text/plain", "Not implemented");
    }
}

void RTWebServer::handleTime()
{}

void RTWebServer::handleBrightness()
{
    if (server.method() == HTTP_GET)
    {
        String brightness = String(this->config.getBrightness());
        server.send(200, "text/plain", brightness);
    }
    else if (server.method() == HTTP_POST)
    {
        this->config.setBrightness(server.arg("plain").toInt());
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(501, "text/plain", "Not implemented");
    }
}

void RTWebServer::handleNet()
{}

void RTWebServer::onNotFound()
{
    Serial.println(F("In 404 handler: ") + server.uri());
    if (LittleFS.exists("/404.html"))
    {
        File file = LittleFS.open("/404.html", "r");

        //Check if open succeded
        if (file)
        {
            String data;
                      
            while (file.available())
            {
                data += char(file.read());
            }
            file.close();

            server.send(404, "text/html", data.c_str());
        }
    }
    else
    {
        server.send(404, "text/plain", "Page not found.");
    }
}

void RTWebServer::handleClient(void)
{
    server.handleClient();
}

RTWebServer::~RTWebServer()
{
    server.stop();
}