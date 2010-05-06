/*
  Sleep.cpp - Arduino Sleep library
  Copyright (c) arms22<arms22 at gmail.com>.  All right reserved.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  Version 003 20 Oct 2009
*/

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <WProgram.h>
#include "Sleep.h"

#define set_sleep_mode_and_sleep(mode)			\
	do {										\
		cli();									\
		set_sleep_mode(mode);					\
		sleep_enable();							\
		sei();									\
		sleep_cpu();							\
		sleep_disable();						\
	}while(0);

void SleepClass::idle(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_IDLE);
}

void SleepClass::noiseReduction(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_ADC);
}

void SleepClass::powerSave(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_PWR_SAVE);
}

void SleepClass::standBy(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_STANDBY);
}

void SleepClass::powerDown(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_PWR_DOWN);
}

void SleepClass::external_event_handler(void){}
void SleepClass::powerDownAndWakeupExternalEvent(uint8_t interruptNumber)
{
	attachInterrupt(interruptNumber, SleepClass::external_event_handler, LOW);
	set_sleep_mode_and_sleep(SLEEP_MODE_PWR_DOWN);
	detachInterrupt(interruptNumber);
}

SleepClass Sleep;

void sleep(unsigned long ms)
{
	unsigned long start = millis();
	while (millis() - start <= ms){
		Sleep.idle();
	}
}
