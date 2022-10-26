 /**
 * NTP LED Clock.
 * 
 * RTWebServer.h
 *
 * Web server handling configuration in running mode.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

#ifndef _RT_WEB_SERVER_H_
#define _RT_WEB_SERVER_H_

class RTWebServer
{
public:
	RTWebServer();

    void start();
    void cleanup();

    static void onWsEvent(AsyncWebSocket * server,
                             AsyncWebSocketClient * client, AwsEventType type,
                             void * arg, uint8_t *data, size_t len);
    void handleCors(AsyncWebServerResponse *response);
    void handleWSData(char *data);

	~RTWebServer();    
private:
	//The web server
    AsyncWebServer server;
    //Web Socket
    AsyncWebSocket ws;

};

#endif