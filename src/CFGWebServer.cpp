#include <CFGWebServer.h>

CFGWebServer::CFGWebServer() : server(80), ws("/ws")
{
    if (!LittleFS.begin())
    {
        Serial.println(F("Error starting LittleFS"));
    }

    // Serve files in directory "/" when request url starts with "/"
    // Request to the root or none existing files will try to server the defualt
    // file name "index.htm" if exists
    this->server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    //Not found (404) page.
    this->server.onNotFound([](AsyncWebServerRequest *request){
        AsyncWebServerResponse* response = request->beginResponse(LittleFS, "/404.html", "text/html");
        response->setCode(404);
        request->send(response);
    });
}

void CFGWebServer::start()
{
    //Start the web server
    this->server.begin();
}

CFGWebServer::~CFGWebServer()
{}