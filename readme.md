# New 303WIFILC01 firmware

This is repository is my development of new firmware for a 7-segment clock I
bought on AliExpress. The default firmware uses NTP, which i found pretty neat,
but it seems to be hardcoded to CST (China Standard Time), which is not neat at
all.

## Acknowledgement

This project would never have begun without finding the work by
*Maartin Pennings*, who have a repository at: 
<https://github.com/maarten-pennings/303WIFILC01>.

## Todo

As always write more documentation.

 * Initial things to study
   *  Write or find some example code for the RTC (DS1302).
   *  Write or find some example code for the display chip (TM1650).
   * Write or find some example code for using NTP.
 * Code design
   * Decide how to tie the RTC in with NTP.
   * Decide how much should be configurable via HTTP
 * Write code 