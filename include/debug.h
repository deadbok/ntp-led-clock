 /**
 * NTP LED Clock.
 * 
 * debug.h
 *
 * Stuff for error handling and debugging.
 *
 * Copyright 2022 Martin Bo Kristensen Grønholdt
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

//Where to ouput the error and warning messages
#define DBDEV   Serial

//Print an error message.
#define error(...)     DBDEV.printf("ERROR: " __VA_ARGS__ )

//Print a warning message.
#define warn(...)     DBDEV.printf("WARN: " __VA_ARGS__)

#endif