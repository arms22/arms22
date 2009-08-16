/*
  Sleep.cpp - Arduino Sleep library
  Copyright (c) arms22<arms22 at gmail.com>.  All right reserved.
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  
  Version 001 16 Jan 2009
*/

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <wiring.h>
#include "Sleep.h"

void Sleep::idle(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_IDLE);
}

void Sleep::noiseReduction(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_ADC);
}

void Sleep::powerSave(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_PWR_SAVE);
}

void Sleep::standBy(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_STANDBY);
}

void Sleep::powerDown(void)
{
	set_sleep_mode_and_sleep(SLEEP_MODE_PWR_DOWN);
}

void Sleep::external_event_handler(void){}
void Sleep::powerDownAndWakeupExternalEvent(uint8_t interruptNumber)
{
	attachInterrupt(interruptNumber, Sleep::external_event_handler, LOW);
	set_sleep_mode_and_sleep(SLEEP_MODE_PWR_DOWN);
	detachInterrupt(interruptNumber);
}

// ISR(WDT_vect){}
// void Sleep::powerDownAndWakeupWatchdogInterrup(uint8_t value)
// {
// 	uint8_t new_reg = _BV(WDIF) | _BV(WDIE) | (value & 0x8 ? _WD_PS3_MASK : 0) | (value & 0x7);
// 	uint8_t old_reg = _WD_CONTROL_REG;
	
// 	cli();
// 	wdt_reset();
// 	MCUSR &= ~_BV(WDRF);
// 	_WD_CONTROL_REG |= _BV(_WD_CHANGE_BIT) | _BV(WDE);
// 	_WD_CONTROL_REG = new_reg;
	
// 	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
// 	sleep_enable();
// 	sei();
// 	sleep_cpu();
// 	sleep_disable();
	
// 	cli();
// 	wdt_reset();
// 	MCUSR &= ~_BV(WDRF);
// 	_WD_CONTROL_REG |= _BV(_WD_CHANGE_BIT) | _BV(WDE);
// 	_WD_CONTROL_REG = old_reg;
// 	sei();
// }
