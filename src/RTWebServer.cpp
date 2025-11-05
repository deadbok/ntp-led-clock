#include "RTWebServer.h"
#include <LittleFS.h>

RTWebServer::RTWebServer(Config& config) : server(80), config(config)
{
}

void RTWebServer::start()
{
    // Serve files needed for the index page.
    server.serveStatic("/", LittleFS, "/index.html");
    server.serveStatic("/index.html", LittleFS, "/index.html");
    server.serveStatic("/css/pico.red.min.css", LittleFS, "/css/pico.red.min.css");
    server.serveStatic("/css/custom.css", LittleFS, "/css/custom.css");
    server.serveStatic("/favicon.ico", LittleFS, "/favicon.ico");
    server.serveStatic("/js/tzdata.js", LittleFS , "/js/tzdata.js");

    // Endpoints for configuration
    server.on("/brightness", std::bind(&RTWebServer::handleBrightness, this));
    server.on("/ntp_state", std::bind(&RTWebServer::handleNTP, this));
    server.on("/ip", std::bind(&RTWebServer::handleIP, this));

    //Not found (404) page.
    server.onNotFound(std::bind(&RTWebServer::onNotFound, this));

    //Read configuration file.
    config.readJSON();

    //Start the web server
    server.begin();
}

void RTWebServer::handleBrightness()
{
    if (server.method() == HTTP_GET)
    {
        String brightness = String(this->config.getBrightness());
        Serial.printf("handleBrightness, GET response, %s\n", brightness.c_str());
        server.send(200, "text/plain", brightness);
    }
    else if (server.method() == HTTP_POST)
    {
        this->config.setBrightness(server.arg("plain").toInt());
        Serial.printf("handleBrightness, POST recieved, %s\n", server.arg("plain"));
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(501, "text/plain", "Not implemented");
    }
}

void RTWebServer::handleNTP()
{
    if (server.method() == HTTP_GET)
    {
        String ntp = String(this->config.getNTPenable());
        Serial.printf("handleNTP, GET response, %s\n", ntp.c_str());
        server.send(200, "text/plain", ntp);
    }
    else if (server.method() == HTTP_POST)
    {
        this->config.setNTPenable(server.arg("plain"));
        Serial.printf("handleNTP, POST recieved, %s\n", server.arg("plain"));
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(501, "text/plain", "Not implemented");
    }
}

void RTWebServer::handleIP()
{
    if (server.method() == HTTP_GET)
    {
        String ip = WiFi.localIP().toString();
        Serial.printf("handleIP, GET response, %s\n", ip.c_str());
        server.send(200, "text/plain", ip);
    }
    else
    {
        server.send(501, "text/plain", "Not implemented");
    }
}

void RTWebServer::onNotFound()
{
    Serial.print(F("In 404 handler: "));
    Serial.println(server.uri());
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
{}