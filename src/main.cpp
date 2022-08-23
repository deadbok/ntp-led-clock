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
#include <TZ.h>
#include <coredecls.h> 
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sntp.h>
#include <ap_secret.h>
#include <hardware.h>
#include <led.h>
#include <TM1650.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <ESP8266TimerInterrupt.h>
#include <CFGWebServer.h>
#include <version.h>

//The instance for the 7-segment display.
TM1650                Display;
//The instance for the RTC.
ThreeWire             RTCWire(DS1302_IO, DS1302_SCLK, DS1302_CE);
RtcDS1302<ThreeWire>  RTC(RTCWire);
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
//Keep track of last minute to determine when to update the display
unsigned char         last_minute;
//Web server for configuring when running.
CFGWebServer          cfgWebServer;

//The NTP servers to sync with
#define NTP_SERVERS "0.pool.ntp.org", "1.pool.ntp.org", "2.pool.ntp.org"

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
    RTC.SetDateTime(cb_rtc_time_date);
  
    Led.toggle();
  }
}

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

  // Initialise the 7-segment display
  Wire.begin(TM1650_SDA, TM1650_SCL);

  Display.init();
  Display.displayOn();

  //Set the callback for when time is set.
  settimeofday_cb(time_is_set);
    
  //Look alive
  for (int i = 0; i < 4; i++)
  {
    Display.setDot(i, true);
  }

  //Initialise the LED
  Led.init();

  //Initialise the RTC.
  RTC.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (RTC.GetIsWriteProtected())
  {
    Serial.println(F("Enabling RTC writing"));
    RTC.SetIsWriteProtected(false);
  }

  if (!RTC.IsDateTimeValid()) 
  {
    Serial.println(F("RTC time is not valid, setting default"));
    RTC.SetDateTime(compiled);
  }

  if (!RTC.GetIsRunning())
  {
    Serial.println(F("Starting RTC"));
    RTC.SetIsRunning(true);
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

  //Set up NTP
  Serial.println(F("Setting up NTP client"));
	configTime(0, 0, NTP_SERVERS);

  //Set time zone for denmark
	setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
	tzset();

  //Setup timer ISR to fire every second
  if (ITimer.attachInterruptInterval(1000 * 1000, timer_handler))
    Serial.println(F("Starting timer"));
  else
    Serial.println(F("Cannot start timer!"));

  cfgWebServer.start();
}

void loop()
{
  struct tm       *time_info;
  RtcDateTime     rtc_now = RTC.GetDateTime();

  if (last_minute != rtc_now.Minute())
  {
    last_minute = rtc_now.Minute();
    time_t now;

    //Get UTC from RTC
    now = rtc_now.Epoch32Time();
    //Convert to local time
    time_info = localtime(&now);
    
    char digits[] = "0000";
    digits[0] = '0' + (time_info->tm_hour / 10);
    digits[1] = '0' + (time_info->tm_hour % 10);
    digits[2] = '0' + (time_info->tm_min / 10);
    digits[3] = '0' + (time_info->tm_min % 10);
   
    Serial.println(digits);
  
    Display.displayString(digits);
  }

  //Toggle the dots if changed
  if (update_dots)
  {
    if (dots) 
      Display.setDot(1, true);
    else
      Display.setDot(1, false);
    
    update_dots = false;
  }
}
