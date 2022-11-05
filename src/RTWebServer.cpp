#include "RTWebServer.h"
#include <LittleFS.h>

RTWebServer::RTWebServer() : server(80)
{}

void RTWebServer::start()
{
    // Serve files needed for the index page.
    server.serveStatic("/", LittleFS, "index.html");
    server.serveStatic("/css/normalize.css", LittleFS, "/css/normalize.css");
    server.serveStatic("/css/custom.css", LittleFS, "/css/custom.css");

    //Not found (404) page.
    server.onNotFound(std::bind(&RTWebServer::onNotFound, this));

    //Start the web server
    server.begin();
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