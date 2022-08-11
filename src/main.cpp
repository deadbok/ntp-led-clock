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


/* Configuration of NTP */
#define MY_NTP_SERVER "dk.pool.ntp.org"

// OPTIONAL: change SNTP startup delay
// a weak function is already defined and returns 0 (RFC violation)
// it can be redefined:
//uint32_t sntp_startup_delay_MS_rfc_not_less_than_60000 ()
//{
//    //info_sntp_startup_delay_MS_rfc_not_less_than_60000_has_been_called = true;
//    return 60000; // 60s (or lwIP's original default: (random() % 5000))
//}

// OPTIONAL: change SNTP update delay
// a weak function is already defined and returns 1 hour
// it can be redefined:
//uint32_t sntp_update_delay_MS_rfc_not_less_than_15000 ()
//{
//    //info_sntp_update_delay_MS_rfc_not_less_than_15000_has_been_called = true;
//    return 15000; // 15s
//}

//Callback when the time is set.
void time_is_set(bool from_sntp)
{
  Serial.print("Time was set ");
  if (from_sntp)
  {
    Serial.print("from SNTP.");
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
  Serial.print(" ");
  Serial.println(__TIME__);

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
    Serial.println("RTC is write protected, enabling writing now");
    RTC.SetIsWriteProtected(false);
  }

  if (!RTC.IsDateTimeValid()) 
  {
    Serial.println("RTC time is not valid.");
    RTC.SetDateTime(compiled);
  }

  if (!RTC.GetIsRunning())
  {
    Serial.println("RTC is not running, starting now");
    RTC.SetIsRunning(true);
  }

  RtcDateTime now = RTC.GetDateTime();
  if (now < compiled) 
  {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    RTC.SetDateTime(compiled);
  }
  else if (now > compiled) 
  {
    Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled) 
  {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
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
	configTime (0, 0, "pool.ntp.org");

  //Set time zone for denmark
	setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
	tzset();

  //Set the initial delay 0 zero to display the time, the first time through the
  //loop.
  waitTime = 0;
}

#define PTM(w) \
  Serial.print(":" #w "="); \
  Serial.print(tm->tm_##w);

void printTm (const char* what, const tm* tm)
{
  Serial.print(what);
  PTM(isdst); PTM(yday); PTM(wday);
  PTM(year);  PTM(mon);  PTM(mday);
  PTM(hour);  PTM(min);  PTM(sec);
}

timeval tv;
struct timezone tz;
timespec tp;
time_t now;

void loop()
{
  if (waitTime < 1)
  {
    gettimeofday (&tv, &tz);
		now = time (nullptr);
    struct tm * timeinfo;
    timeinfo = localtime (&now);
    char digits[] = "0000";

    digits[0] = '0' + (timeinfo->tm_hour / 10);
    digits[1] = '0' + (timeinfo->tm_hour % 10);
    digits[2] = '0' + (timeinfo->tm_min / 10);
    digits[3] = '0' + (timeinfo->tm_min % 10);

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

  /*gettimeofday (&tv, &tz);
	//clock_gettime (0, &tp);	// also supported by esp8266 code
	tnow = time (nullptr);

	// localtime / gmtime every second change
	static time_t lastv = 0;
	if(lastv != tv.tv_sec)
	{
		lastv = tv.tv_sec;
		printf ("tz_minuteswest: %d, tz_dsttime: %d\n",
			tz.tz_minuteswest, tz.tz_dsttime);
		printf ("gettimeofday() tv.tv_sec : %lld\n", lastv);
		printf ("time()            time_t : %lld\n", tnow);
		Serial.println ();

		printf ("         ctime: %s", ctime (&tnow));	// print formated local time
		printf (" local asctime: %s", asctime (localtime (&tnow)));	// print formated local time
		printf ("gmtime asctime: %s", asctime (gmtime (&tnow)));	// print formated gm time

		// print gmtime and localtime tm members
		printTm ("      gmtime", gmtime (&tnow));
		Serial.println ();
		printTm ("   localtime", localtime (&tnow));
		Serial.println ();

		Serial.println ();
	}*/
}
