/**
 * NTP LED Clock.
 * 
 * clock.h
 *
 * Clock class declaration.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <TM1637.h>
#include <Wire.h>  
#include <RtcDS3231.h>

#ifndef _NRCLOCK_H_
#define _NRCLOCK_H_

//The NTP servers to sync with
#define NTP_SERVERS "0.pool.ntp.org", "1.pool.ntp.org", "2.pool.ntp.org"
//The NTP sync interval in ms
#define NTP_SYNC_INTERVAL 1000*60*60*12 // 12 hours.
class NRClock
{
private:
    //The instance for the 7-segment display.
    TM1637                display;
    //The instance for the RTC.
    RtcDS3231<TwoWire>    RTC;
    //Keep track of last minute to determine when to update the display
    unsigned char         last_minute;

public:
                    NRClock();
    void            init();
    void            update();
	void	        dots(bool state);
	void 	        setDateTime(RtcDateTime datetime);
    unsigned char   getBrightness();
    void            setBrightness(unsigned char val);
                    ~NRClock();
};

#endif