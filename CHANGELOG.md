# ChangeLog

## 13-05-2024

* src/main.cpp:             Added untested OTA code.
* src/NRClock.cpp:          Working on correct output to LCD.
* platformio.ini:           Added OTA target "lolin32-OTA".
* *OTA works.*

## 14-05-2024

* src/NRClock.cpp:          Working on correct output to LCD.
* *Display seems to display time correctly.*
* src/Config.cpp:           Added option to toggle of NTP sync.
* include/Config.h:         Added option to toggle of NTP sync.
* src/RTWebServer.cpp:      Exposed option NTP sync.
* include/RTWebServer.h:    Exposed option NTP sync.

## 25-05-2024

Working on html frontend.

* Uses <https://picocss.com/> (Pico CSS).
* Added ArduinoJson library dependency.
* data/index.html: Static page is somewhat laid out.

## 26-05-2024

Working on html frontend.

* data/index.html:
  * Added getBrightness().
  * Added getNTP().
  * Added Status card.
  * Added listener for DOMContentLoaded event.
* data/css/custom.css: Added classes for status card.

## 01-06-2024

Working on saving configuration to flash.

* include/Config.h:
  * Added readJSON.
  * Added writeJSON.
  * Added updateJSON.
  * Added updateFromJson.
* src/Config.cpp:
  * Added 'changed' update to every setter.
  * Added readJSON.
  * Added writeJSON.
  * Added updateJSON.
  * Added updateFromJson.
* README.md: Updated progress on ESP32 porting.

## 05-06-2024

Working on configuration "backend".

* platformio.ini: Removed debug ouput from "lolin32" target.
* inlude/Config.h:
  * Removed uneeded define
  * Added const config_fn to hold the config filename.
* src/Config.cpp:
  * Added writeJSON to setters
  * Added readJSON to constructor.
* include/RTWebServer.h: Added handleIP.
* src/RTWebServer.cpp:
  * Added "/ip" endpoint to get the IP address
  * Added handleIP.
  * Read config on construction.
* data/index.html: Added getIP to update the clock ip address in the frontend.

## 06-06-2024

* /src/RTWebServer.cpp: Fixed to int conversion in handleNTP

## 12-10-2025

* tools/test_server: Working on a flask emulation of the firmware.
* tools/app.py:
  * Sending json formatted data to the web interface, all to one "/config" endpoint.
* data/index.html:
  * Added get_config to requst config from the webserver
  * Added config_listener to update web interface from a JSON response.
  * Added post_config to sent config data to the webserver.
  * Added config_proxy to sent config when changed.
  
## 13-10-2025

* Removed old post/get functions all handled by post_config/get_config.

## 14-10-2025

* data/js/zones.json: Updated from https://github.com/nayarsystems/posix_tz_db
* data/index.html:
  * Added loading timezones from zones.json.

## 15-10-2025

* data/index.html:
  * Selection of timezone now done.

## 5-11-2025

* data/index.html:
  * Selection of timezone now done. For real.