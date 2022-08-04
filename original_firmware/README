# Backup of original firmware

This directoy contains the original firmware in *backup.bin*.

## Terminal output when dumping the firmware

    python3 ~/.platformio/packages/tool-esptoolpy/esptool.py --port /dev/ttyUSB0 --baud 115200 read_flash 0x0000 1048576 backup.bin
    esptool.py v2.8
    Serial port /dev/ttyUSB0
    Connecting....
    Detecting chip type... ESP8266
    Chip is ESP8266EX
    Features: WiFi
    Crystal is 26MHz
    MAC: 40:f5:20:22:a0:79
    Uploading stub...
    Running stub...
    Stub running...
    1048576 (100 %)
    1048576 (100 %)
    Read 1048576 bytes at 0x0 in 95.0 seconds (88.3 kbit/s)...
    Hard resetting via RTS pin...

## Geeting flash chip information

    python3 ~/.platformio/packages/tool-esptoolpy/esptool.py --port /dev/ttyUSB0 --baud 115200 flash_id
    esptool.py v2.8
    Serial port /dev/ttyUSB0
    Connecting....
    Detecting chip type... ESP8266
    Chip is ESP8266EX
    Features: WiFi
    Crystal is 26MHz
    MAC: 40:f5:20:22:a0:79
    Uploading stub...
    Running stub...
    Stub running...
    Manufacturer: 85
    Device: 6014
    Detected flash size: 1MB
    Hard resetting via RTS pin...

