/*
  Sleep.h - Arduino Sleep library
  Copyright (c) arms22<arms22 at gmail.com>.  All right reserved.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  Version 002 19 Jan 2009
*/

#ifndef Sleep_h
#define Sleep_h

#include <inttypes.h>
#include <avr/wdt.h>

#define set_sleep_mode_and_sleep(mode)			\
	do {										\
		cli();									\
		set_sleep_mode(mode);					\
		sleep_enable();							\
		sei();									\
		sleep_cpu();							\
		sleep_disable();						\
	}while(0);

class Sleep
{
	Sleep(){}
	static void external_event_handler(void);
	
public:
	~Sleep(){}
	static void idle(void);
	static void noiseReduction(void);
	static void powerSave(void);
	static void standBy(void);
	static void powerDown(void);
	static void powerDownAndWakeupExternalEvent(uint8_t interruptNumber);
/* 	static void powerDownAndWakeupWatchdogInterrup(uint8_t value); */
};

#endif
