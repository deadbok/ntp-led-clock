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

    // Endpoints for configuration
    server.on("/brightness", std::bind(&RTWebServer::handleBrightness, this));

    //Not found (404) page.
    server.onNotFound(std::bind(&RTWebServer::onNotFound, this));

    //Start the web server
    server.begin();
}

void RTWebServer::handleBrightness()
{
    Serial.printf("In brightness handler: %d ", server.method());
    if (server.method() == HTTP_GET)
    {
        Serial.println("GET");

        server.send(200, "text/plain", "OK");
    }
    else if (server.method() == HTTP_POST)
    {
        Serial.println("PUT");
        Serial.printf("Recived: %s\n", server.arg("plain").c_str());

        this->config.setBrightness(atoi(server.arg("plain").c_str()));

        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(501, "text/plain", "Not implemented");
    }
}

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
{}