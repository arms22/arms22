#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <inttypes.h>
#include <avr/wdt.h>

class WatchdogClass
{
	uint8_t _controlReg;
	
public:
	enum {
		TimeOut15ms = WDTO_15MS,
		TimeOut30ms = WDTO_30MS,
		TimeOut60ms = WDTO_60MS,
		TimeOut120ms = WDTO_120MS,
		TimeOut250ms = WDTO_250MS,
		TimeOut500ms = WDTO_500MS,
		TimeOut1s = WDTO_1S,
		TimeOut2s = WDTO_2S,
#if defined(WDP3)
		TimeOut4s = WDTO_4S,
		TimeOut8s = WDTO_8S,
#endif
	};
	WatchdogClass();
	~WatchdogClass(){}
	static void attachInterrupt(void(*handler)(void));
	static void detachInterrupt(void);	
	void systemResetEnable(bool flag);
	void enable(uint8_t value);
	static void disable(void){
		wdt_disable();
	}
	static void timerReset(void){
		wdt_reset();
	}
};

extern WatchdogClass Watchdog;

#endif
