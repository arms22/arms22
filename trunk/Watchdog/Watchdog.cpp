#include <avr/interrupt.h>
#include <wiring.h>
#include "Watchdog.h"

static void (*wdtIntHandler)(void) = 0;
uint8_t Watchdog::_controlReg = _BV(WDIF) | _BV(WDIE) | _BV(WDE) | _BV(WDP2);

void Watchdog::attachInterrupt(void(*handler)(void))
{
	wdtIntHandler = handler;
}

void Watchdog::detachInterrupt(void)
{
	wdtIntHandler = 0;
}

void Watchdog::systemResetEnable(bool flag)
{
	if(flag){
		_controlReg |= _BV(WDE);
	}else{
		_controlReg &= ~_BV(WDE);
	}
}

void Watchdog::enable(uint8_t value)
{
	uint8_t reg = _controlReg | (value & 0x8 ? _WD_PS3_MASK : 0) | (value & 0x7);
	cli();
	wdt_reset();
	MCUSR &= ~_BV(WDRF);
	_WD_CONTROL_REG |= _BV(_WD_CHANGE_BIT) | _BV(WDE);
	_WD_CONTROL_REG = reg;
	sei();
}

ISR(WDT_vect)
{
	if(wdtIntHandler){
		wdtIntHandler();
	}
}
