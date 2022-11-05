 /**
 * NTP LED Clock.
 * 
 * RTWebServer.h
 *
 * Web server handling configuration in running mode.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <ESP8266WebServer.h>

#ifndef _RT_WEB_SERVER_H_
#define _RT_WEB_SERVER_H_

class RTWebServer
{
public:
	RTWebServer();

    void start();

    void onNotFound();

    void handleClient(void);

	~RTWebServer();    
private:
	//The web server
    ESP8266WebServer server;
};

#endif