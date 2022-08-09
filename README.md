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

## Todo

As always write more documentation.

  - [x] Initial things to study
    - [x] Write or find some example code for the RTC (DS1302).
    - [x] Write or find some example code for the display chip (TM1650).
    - [x] Write or find some example code for using NTP.
  - [ ] Code design
    - [ ] Decide how to tie the RTC in with NTP.
    - [ ] Decide how much should be configurable via HTTP
    - [ ] Decide how to use the external flash.
  - [ ] Write code 