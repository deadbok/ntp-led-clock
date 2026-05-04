#include "RTWebServer.h"
#include <LittleFS.h>

RTWebServer::RTWebServer(Config& config) : server(80), config(config)
{
}

void RTWebServer::start()
{
    // Serve files needed for the webinterface page.
    server.serveStatic("/", LittleFS, "/index.html");
    server.serveStatic("/index.html", LittleFS, "/index.html");
    server.serveStatic("/wireless.html", LittleFS, "/wireless.html");
    server.serveStatic("/about.html", LittleFS, "/about.html");
    server.serveStatic("/css/pico.red.min.css", LittleFS, "/css/pico.red.min.css");
    server.serveStatic("/css/custom.css", LittleFS, "/css/custom.css");
    server.serveStatic("/favicon.ico", LittleFS, "/favicon.ico");
    server.serveStatic("/js/zones.json", LittleFS , "/js/zones.json");

    // Endpoints for configuration
    server.on("/config", std::bind(&RTWebServer::handleConfig, this));
    server.on("/ssids", std::bind(&RTWebServer::handleSSIDs, this));

    //Not found (404) page.
    server.onNotFound(std::bind(&RTWebServer::onNotFound, this));

    //Read configuration file.
    //config.readJSON();

    //Start the web server
    server.begin();
}

// Send or receive a JSON configuration
void RTWebServer::handleConfig()
{
    if (server.method() == HTTP_GET)
    {
        String config = this->config.getJSON();
#ifdef DEBUG
        Serial.printf("handleConfig, GET response, %s\n", config.c_str());
#endif
        server.send(200, "application/json", config);
    }
    else if (server.method() == HTTP_POST)
    {
#ifdef DEBUG
        Serial.printf("handleConfig, POST received: %s\n", server.arg("plain").c_str());
#endif
        // Set configuration from the received data.
        this->config.setFromJSON(server.arg("plain"));
        // Save configuration.
		this->config.writeJSON();
        server.send(200, "text/plain", "OK");
    }
    else
    {
        server.send(501, "text/plain", "Not implemented");
    }
}

// Send all detected SSIDs 
void RTWebServer::handleSSIDs()
{
    if (server.method() == HTTP_GET)
    {
		// Create an empty JSON array
		JsonDocument doc;
		JsonArray ssids = doc.to<JsonArray>();

		Serial.print("Scanning WiFi networks...");
    
        int n = WiFi.scanNetworks();
        
        if (n == 0) 
        {
            Serial.println("no networks found");
        }
        else
        {
            Serial.printf("%d networks found", n);

            for (int i = 0; i < n; ++i)
			{
				String ssid = WiFi.SSID(i);

          		Serial.printf("%2d: %s\n", i + 1, ssid.c_str());
				ssids.add(ssid);
      		}
	    }

		
    	String  serialJSON;
	    serializeJson(doc, serialJSON);

#ifdef DEBUG
        Serial.printf("handleSSIDs, GET response, %s\n", serialJSON.c_str());
#endif
        server.send(200, "application/json", serialJSON);
    }
    else
    {
        server.send(501, "text/plain", "Not implemented");
    }   
}

// Error handler, a requested URL does not exist.
void RTWebServer::onNotFound()
{
    Serial.printf("In 404 handler: %s\n", server.uri().c_str());
    if (LittleFS.exists("/404.html"))
    {
        File file = LittleFS.open("/404.html", "r");

        //Check if open succeeded
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