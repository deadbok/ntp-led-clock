# ChangeLog

## 13-05-2024

* src/main.cpp:             Added untested OTA code.
* src/NRClock.cpp:          Working on correct output to LCD.
* platformio.ini:           Added OTA target "lolin32-OTA".
* *OTA works.*

## 14-05-2024

* src/NRClock.cpp:          Working on correct output to LCD.
* *Display seems to display time correctly."
* src/Config.cpp:           Added option to toggle of NTP sync.
* include/Config.h:         Added option to toggle of NTP sync.
* src/RTWebServer.cpp:      Exposed option NTP sync.
* include/RTWebServer.h:    Exposed option NTP sync.

## 25-05-2024

Working on html frontend.

 * Uses <https://picocss.com/> (Pico CSS).
 * Added ArduinoJdon library dependency.
 
 * data/index.html: Static page is somewhat laid out.

 ## 26-05-2024

Working on html frontend.

 * data/index.html: Added getBrightness().
                    Added getNTP().
                    Added Status card.
* data/css/custom.css: Added classes for status card.
