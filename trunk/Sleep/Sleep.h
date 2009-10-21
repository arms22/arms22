/*
  Sleep.h - Arduino Sleep library
  Copyright (c) arms22<arms22 at gmail.com>.  All right reserved.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  Version 003 20 Oct 2009
*/

#ifndef Sleep_h
#define Sleep_h

#include <inttypes.h>
#include <avr/wdt.h>

class SleepClass
{
	static void external_event_handler(void);
	
public:
	static void idle(void);
	static void noiseReduction(void);
	static void powerSave(void);
	static void standBy(void);
	static void powerDown(void);
	static void powerDownAndWakeupExternalEvent(uint8_t interruptNumber);
};

extern SleepClass Sleep;

#endif
