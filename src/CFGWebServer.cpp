#include <CFGWebServer.h>

CFGWebServer::CFGWebServer() : server(80), ws("/ws")
{
    if (!LittleFS.begin())
    {
        Serial.println(F("Error starting LittleFS"));
    }

    // Serve files in directory "/www/" when request url starts with "/"
    // Request to the root or none existing files will try to server the defualt
    // file name "index.htm" if exists
    this->server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
}

void CFGWebServer::start()
{
    //Start the web server
    this->server.begin();
}

CFGWebServer::~CFGWebServer()
{}