 /**
 * NTP LED Clock.
 * 
 * hardware.h
 *
 * Defines for accessing the hardware on the board.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

//DS3231 RTC
#define DS3231_SDA      21
#define DS3231_SCL      22

//TM1650 display driver
#define TM1650_SCL      22
#define TM1650_SDA      21

//LED Next to the esp8266 on the board
#define LED_PIN         2

//The buttons on the board
#define BUTTON_1        1
#define BUTTON_2        2
#define BUTTON_3        4

#endif