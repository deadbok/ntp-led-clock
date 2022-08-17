 /**
 * NTP LED Clock.
 * 
 * main.cpp
 *
 * Main program file.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <TZ.h>
#include <coredecls.h> 
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <ap_secret.h>
#include <hardware.h>
#include <TM1650.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <version.h>

//The variable for the 7-segment display.
TM1650                Display;
//The variables for the RTC.
ThreeWire             RTCWire(DS1302_IO, DS1302_SCLK, DS1302_CE);
RtcDS1302<ThreeWire>  RTC(RTCWire);
//Variable used for delays.
unsigned int          waitTime;


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
  Serial.print("Time was set ");
  if (from_sntp)
  {
    Serial.print("from SNTP.");
    //Update RTC time.
  }
  Serial.println();
}

void setup()
{
  //Greetings on the serial port.
  Serial.begin(115200);
  Serial.printf("\nNTP LED Clock %s.\n", VERSION);
  Serial.print("Compiled: ");
  Serial.print(__DATE__);

  // Initialise the 7-segment display
  Wire.begin(TM1650_SDA, TM1650_SCL);

  Display.init();
  Display.displayOn();

  //Set the callback for when time is set.
  settimeofday_cb(time_is_set);
    
  //Cycle the dots on the display to tell we are alive.
  for (int i = 0; i < 4; i++)
  {
    Display.setDot(i, true);
    delay(300);
    Display.setDot(i, false);
    delay(200);
  }

  //Initialise the RTC.
  RTC.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

  if (RTC.GetIsWriteProtected())
  {
    Serial.println("Enabling RTC writing");
    RTC.SetIsWriteProtected(false);
  }

  if (!RTC.IsDateTimeValid()) 
  {
    Serial.println("RTC time is not valid.");
    RTC.SetDateTime(compiled);
  }

  if (!RTC.GetIsRunning())
  {
    Serial.println("Starting RTC.");
    RTC.SetIsRunning(true);
  }

  //Connect to the network
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);
  Serial.print("Connect");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(200);
  }
  Serial.println(F("WiFi connected"));

  //Set up NTP
	configTime (0, 0, NTP_SERVERS);

  //Set time zone for denmark
	setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
	tzset();

  //Set the initial delay 0 zero to display the time, the first time through the
  //loop.
  waitTime = 0;
}

void loop()
{
  struct tm       *time_info;
  struct timezone tz;
  time_t          now;
  timeval         tv;


  if (waitTime < 1)
  {
    gettimeofday(&tv, &tz);
		now = time(nullptr);
    time_info = localtime(&now);
    
    char digits[] = "0000";

    digits[0] = '0' + (time_info->tm_hour / 10);
    digits[1] = '0' + (time_info->tm_hour % 10);
    digits[2] = '0' + (time_info->tm_min / 10);
    digits[3] = '0' + (time_info->tm_min % 10);

    Serial.println(digits);
  
    Display.displayString(digits);
    //Reset delay.
    waitTime = 60;
  }

  //Blink the middle dots to look alive.
  Display.setDot(1, true);
  delay(400);
  Display.setDot(1, false);
  delay(600);

  //Decrease time before updating the time.
  waitTime--;
}
