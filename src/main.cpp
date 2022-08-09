 /**
 * NTP LED Clock.
 * 
 * main.cpp
 *
 * Main program file.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */

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

void setup()
{
  //Greetings on the serial port.
  Serial.begin(115200);
  Serial.printf("\nNTP LED Clock %s.\n", VERSION);
  Serial.print("Compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  // Initialise the 7-segment display
  Wire.begin(TM1650_SDA, TM1650_SCL);

  Display.init();
  Display.displayOn();
    
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

  //Set the initial delay 0 zero to display the time, the first time through the
  //loop.
  waitTime = 0;
}

void loop()
{
  if (waitTime < 1)
  {
    RtcDateTime now = RTC.GetDateTime();
    char digits[] = "0000";

    digits[0] = '0' + (now.Hour() / 10);
    digits[1] = '0' + (now.Hour() % 10);
    digits[2] = '0' + (now.Minute() / 10);
    digits[3] = '0' + (now.Minute() % 10);

//    snprintf_P(digits, sizeof(digits), PSTR("%02u:%02u"), now.Hour(),
//                now.Minute());

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
