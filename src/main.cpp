 /**
 * NTP LED Clock.
 * 
 * main.cpp
 *
 * Main program file.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
//For no timer debug ouput.
#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
//For longest timer
#define USING_TIM_DIV256              true

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <RtcDateTime.h>
#include <coredecls.h> 
#include <stdlib.h>
#include <sntp.h>
#include <ESP8266TimerInterrupt.h>
#include <LittleFS.h>
#include "CFGWebServer.h"
#include "ap_secret.h"
#include "hardware.h"
#include "led.h"
#include "nrclock.h"
#include "version.h"

//The instance to control the LED
LED                   Led(LED_PIN);
//Init ESP8266 timer 1
ESP8266Timer          ITimer;
//Variable used in the ISR for counting seconds
volatile unsigned int ISR_seconds;
//Update dots
volatile bool         update_dots;
//Dots on/off
volatile bool         dots;
//The NTP & RTC clock 
NRClock               nrclock;
//Web server for configuring when running.
CFGWebServer          cfgWebServer;

//Set the delay between SNTP updates to 12 hours.
uint32_t sntp_update_delay_MS_rfc_not_less_than_15000()
{
	return(1000*60*60*12); // 12 hours.
}

//Callback when the time is set.
void time_is_set(bool from_sntp)
{
	RtcDateTime cb_rtc_time_date(0);
	timeval     cb_time;

	Serial.println(F("Time was set"));
  	if (from_sntp)
  	{
    	Led.toggle();
    	Serial.println(F("Updating RTC time from SNTP"));
  
    	//Get time set by NTP
    	gettimeofday(&cb_time, NULL);
    	//Update RTC with UTC time from NTP
    	cb_rtc_time_date.InitWithEpoch32Time(cb_time.tv_sec);
		//Set clock from NTP
    	nrclock.SetDateTime(cb_rtc_time_date);
  
    	Led.toggle();
  	}
}

//Handler for the timer interrupt
void IRAM_ATTR timer_handler()
{
  	//Increase seconds.
  	ISR_seconds++;

  	//Toggle the dots
  	dots = !dots;
  	//Update the dots
  	update_dots = true;

  	//Reset seconds every 12 hours
  	if (ISR_seconds == (60 * 60 * 12))
    	ISR_seconds = 0;
}

void setup()
{
	//Greetings on the serial port.
  	Serial.begin(115200);
  	Serial.println(F(""));
  	Serial.print(F("NTP LED Clock v"));
  	Serial.println(F(VERSION));
  	Serial.print(F("Compiled: "));
  	Serial.println(F(__DATE__));

  	//Set the callback for when time is set.
  	settimeofday_cb(time_is_set);
    
   	//Initialise the LED
  	Led.init();

    //Init filesystem
  	if (!LittleFS.begin())
  	{
    	Serial.println(F("Error starting LittleFS"));
  	}

  	//Connect to the network
  	WiFi.mode(WIFI_STA);
  	WiFi.begin(STASSID, STAPSK);
  	Serial.print(F("Connect"));
  	while (WiFi.status() != WL_CONNECTED)
  	{
    	Led.toggle();
    	Serial.print(F("."));
    	delay(200);
  	}
  	Serial.println(F(""));
  	Serial.print(F("WiFi connected: "));
  	Serial.println(STASSID);
  	Serial.print(F("IP address: "));
  	Serial.println(WiFi.localIP());
  	Led.on();

  	//Setup timer ISR to fire every second
  	if (ITimer.attachInterruptInterval(1000 * 1000, timer_handler))
    	Serial.println(F("Starting timer"));
  	else
    	Serial.println(F("Cannot start timer!"));

	//Init clock
  	nrclock.init();

  	cfgWebServer.start();
}

void loop()
{
	//Update clock display
	nrclock.update();

  	//Toggle the dots if changed
  	if (update_dots)
  	{
    	nrclock.dots(dots);
    
    	update_dots = false;
  	}

  	//Do housekeeping of the webserver
  	cfgWebServer.cleanup();
}
