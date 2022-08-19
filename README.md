# New 303WIFILC01 firmware

This is repository is my development of new firmware for a 7-segment clock I
bought on AliExpress. The default firmware uses NTP, which i found pretty neat,
but it seems to be hardcoded to CST (China Standard Time), which is not neat at
all.

## Acknowledgement

This project would never have begun without finding the work by
*Maartin Pennings*, who have a repository at: 
<https://github.com/maarten-pennings/303WIFILC01>.
Some good documentation on the NTP services in the ESP8266 can be found at:
<https://werner.rothschopf.net/202011_arduino_esp8266_ntp_en.htm> and
<https://werner.rothschopf.net/microcontroller/202112_arduino_esp_ntp_rtc_en.htm>
and a really helpful thread at:
<https://forum.arduino.cc/t/getting-time-from-ntp-service-using-nodemcu-1-0-esp-12e/702333>.

## Time sources

The clock is using the RTC and the NTP to keep the clock current.

  - When the clock initialises it starts running from the RTC
  - If a connection to the internet is established it updates the RTC from NTP
  - The RTC will continue to update from NTP every 12 hours
  - RTC runs on UTC

## Configuration

The clock has a built-in web server to configure:

  - WiFi access configuration (SSID & PSK)
  - The NTP server to use
  - The time zone

## Y2K38

*At the time this code was written, the ESP8266 time_t value uses signed 32 bit.*
*That means that the esp8266 time functions will suffer from the the Y2K38*
*problem: <https://en.wikipedia.org/wiki/Year_2038_problem>.*
*This means that the esp8266 time library code will malfunction starting*
*03:14:07 on Tuesday, 19 January 2038 GMT.*

## To do

**As always write more documentation.**

  - [x] Initial things to study
    - [x] Write or find some example code for the RTC (DS1302).
    - [x] Write or find some example code for the display chip (TM1650).
    - [x] Write or find some example code for using NTP.
  - [ ] Code design
    - [x] Decide how to tie the RTC in with NTP.
    - [x] Decide how much should be configurable via HTTP
    - [ ] Decide how to use the flash memory.
  - [ ] Write code
    - [x] Time display
    - [x] RTC code
    - [x] NTP code
    - [x] Make use of the LED on the back og the board.
    - [ ] Put WiFi to sleep when unused
    - [x] Update the display via timer interrupt
    - [ ] Web server code
    - [ ] *Remember* 12 hour mode
    - [ ] Rechargeable battery support
    - [ ] Web page for configuration
      - [ ] Configuration from web page to and from clock
      - [ ] Set AP mode when no WiFi connection could be established
      - [ ] Get time-zone from browser