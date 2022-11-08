/**
 * NTP LED Clock.
 * 
 * clock.h
 *
 * Clock class declaration.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */
#include <TM1650.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>

#ifndef _NRCLOCK_H_
#define _NRCLOCK_H_

//The NTP servers to sync with
#define NTP_SERVERS "0.pool.ntp.org", "1.pool.ntp.org", "2.pool.ntp.org"

class NRClock
{
private:
    //The instance for the 7-segment display.
    TM1650                display;
    //The instance for the RTC.
    ThreeWire             RTCWire;
    RtcDS1302<ThreeWire>  RTC;
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