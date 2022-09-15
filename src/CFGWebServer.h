 /**
 * NTP LED Clock.
 * 
 * CFGWebServer.h
 *
 * Web server handling configuration in running mode.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>

#ifndef _CFG_WEB_SERVER_H_
#define _CFG_WEB_SERVER_H_

class CFGWebServer
{
public:
	CFGWebServer();

    void start();
    void cleanup();

    static void onNTPWsEvent(AsyncWebSocket * server,
                             AsyncWebSocketClient * client, AwsEventType type,
                             void * arg, uint8_t *data, size_t len);

	~CFGWebServer();    
private:
	//The web server
    AsyncWebServer server;
    //Web Socket
    AsyncWebSocket NTPws;
};

#endif