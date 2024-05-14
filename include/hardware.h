 /**
 * NTP LED Clock.
 * 
 * hardware.h
 *
 * Defines for accessing the hardware on the board.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <Esp.h>

#ifndef _HARDWARE_H_
#define _HARDWARE_H_

//DS3231 RTC
#define DS3231_SDA      22
#define DS3231_SCL      21

//TM1650 display driver
#define TM1637_SCL      23
#define TM1637_SDA      19
//Number of digits on the display
#define N_DIGITS        4

//LED Next to the esp8266 on the board
#define LED_PIN         5

//The buttons on the board
#define BUTTON_1        1
#define BUTTON_2        2
#define BUTTON_3        4

unsigned long chipID(void);

#endif