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

// DS1302 RTC
#define DS1302_SCLK  16
#define DS1302_CE    05
#define DS1302_IO    14

// TM1650 display driver
#define TM1650_SCL 12
#define TM1650_SDA 13

#endif