 /**
 * NTP LED Clock.
 * 
 * RTWebServer.h
 *
 * Web server handling configuration in running mode.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <WebServer.h>
#include "Config.h"

#ifndef _RT_WEB_SERVER_H_
#define _RT_WEB_SERVER_H_

class RTWebServer
{
public:
	RTWebServer(Config& config);

    void start();

    void onNotFound();

    void handleBrightness();

    void handleClient(void);

	~RTWebServer();    
private:
	//The web server
    WebServer    server;
    //The config.
    Config&             config;
};

#endif