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
#include <WiFi.h>
#include <RtcDS3231.h>
//#include <coredecls.h> 
#include <stdlib.h>
#include <esp_sntp.h>
#include <LittleFS.h>
#include "Config.h"
#include "RTWebServer.h"
#include "ap_secret.h"
#include "hardware.h"
#include "led.h"
#include "NRClock.h"
#include "Config.h"
#include "version.h"

//The instance to control the LED
LED                   		Led(LED_PIN);
//Timer structure
hw_timer_t 					*timer = NULL;
//Used to keep track of critical sections
portMUX_TYPE 				timer_mux = portMUX_INITIALIZER_UNLOCKED;
//Variable used in the ISR for counting seconds
volatile unsigned int 		ISR_seconds;
//Update dots
volatile bool         		update_dots;
//Dots on/off
volatile bool         		dots;
//The NTP & RTC clock 
NRClock               		nrclock;
//Configuration data and handling.
Config						config(nrclock);
//Web server for configuring when running.
RTWebServer					runtimeWebServer(config);

 
//Set the delay between SNTP updates to 12 hours.
uint32_t sntp_update_delay_MS_rfc_not_less_than_15000()
{
	return(1000*60*60*12); // 12 hours.
}

//Callback when the time is set.
void time_is_set_sntp(struct timeval *cb_time)
{
	RtcDateTime cb_rtc_time_date(0);

	Serial.println(F("Time was set"));
   	Led.toggle();
   	Serial.println(F("Updating RTC time from SNTP"));
  
   	//Get time set by NTP
   	gettimeofday(cb_time, NULL);
   	//Update RTC with UTC time from NTP
   	cb_rtc_time_date.InitWithUnix32Time(cb_time->tv_sec);
	//Set clock from NTP
   	nrclock.setDateTime(cb_rtc_time_date);
  
   	Led.toggle();
}

//Handler for the timer interrupt
void ARDUINO_ISR_ATTR timer_handler()
{
	portENTER_CRITICAL_ISR(&timer_mux);
  	//Increase seconds.
  	ISR_seconds++;

  	//Toggle the dots
  	dots = !dots;
  	//Update the dots
  	update_dots = true;

  	//Reset seconds every 12 hours
  	if (ISR_seconds == (60 * 60 * 12))
    	ISR_seconds = 0;

	portEXIT_CRITICAL_ISR(&timer_mux);
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
  	sntp_set_time_sync_notification_cb(time_is_set_sntp);
    
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

	// Set timer frequency to 1Mhz
	// Timer 0
	// Prescaler 80
	// Count up
	timer = timerBegin(0, 80, true);
	if (timer == NULL)
	{
    	Serial.println(F("Error starting timer 0"));
  	}

	// Attach onTimer function to our timer.
	timerAttachInterrupt(timer, &timer_handler, true);

  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);

	//Init clock
  	nrclock.init();

  	runtimeWebServer.start();
}

void loop()
{
	//Update clock display
	nrclock.update();

  	//Toggle the dots if changed
	portENTER_CRITICAL(&timer_mux);
  	if (update_dots)
  	{
    	nrclock.dots(dots);
    
    	update_dots = false;
	}
	portEXIT_CRITICAL(&timer_mux);

	runtimeWebServer.handleClient();
}
