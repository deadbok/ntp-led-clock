#include "CFGWebServer.h"
#include <LittleFS.h>

CFGWebServer::CFGWebServer() : server(80), NTPws("/ws/ntp")
{}

void CFGWebServer::start()
{
    // Serve files in directory "/" when request url starts with "/"
    // Request to the root or none existing files will try to server the default
    // file name "index.htm" if exists
    this->server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    //Add handler for NTP WebSockets
    NTPws.onEvent(this->onNTPWsEvent);
    this->server.addHandler(&NTPws);

    //Not found (404) page.
    this->server.onNotFound([](AsyncWebServerRequest *request){
        AsyncWebServerResponse* response = request->beginResponse(LittleFS, 
                                                                  "/404.html",
                                                                  "text/html");
        response->setCode(404);
        Serial.print("404 on: ");
        Serial.println(request->url());
        request->send(response);
    });

    //Start the web server
    this->server.begin();
}

 void CFGWebServer::cleanup()
 {
    NTPws.cleanupClients();
 }

void CFGWebServer::onNTPWsEvent(AsyncWebSocket * server,
                                AsyncWebSocketClient * client,
                                AwsEventType type, void * arg, uint8_t *data,
                                size_t len)
{
    switch(type)
    {
        case WS_EVT_CONNECT:
            Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
            break;
        case WS_EVT_DISCONNECT:
            Serial.printf("ws[%s][%u] disconnect\n", server->url(),
                          client->id());
            break;
        case WS_EVT_ERROR:
            Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(),
                          client->id(), *((uint16_t*)arg), (char*)data);
            break;
        case WS_EVT_PONG:
            Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(),
                          client->id(), len, (len)?(char*)data:"");
            break;
        case WS_EVT_DATA:
            AwsFrameInfo * info = (AwsFrameInfo*)arg;
            if(info->final && info->index == 0 && info->len == len)
            {
                if (info->opcode == WS_TEXT)
                {
                    Serial.printf("Data: %s", data);
                }
                else
                    Serial.println(F("Received something unexpected!"));
            }
            break;
    } 
}

CFGWebServer::~CFGWebServer()
{}