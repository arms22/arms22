#ifndef RTC8564_h
#define RTC8564_h

#include <Arduino.h>

class RTC8564
{
private:
	void init(void);
	uint8_t _seconds;
	uint8_t _minutes;
	uint8_t _hours;
	uint8_t _days;
	uint8_t _weekdays;
	uint8_t _months;
	uint8_t _years;
	bool	_century;
	
public:
	enum {
		BCD = 0,
		Decimal = 1,
	};
	RTC8564();
	void begin(void);
	void sync(uint8_t date_time[],uint8_t size = 7);
	bool available(void);
	bool isvalid(void);
	uint8_t seconds(uint8_t format = RTC8564::BCD) const;
	uint8_t minutes(uint8_t format = RTC8564::BCD) const;
	uint8_t hours(uint8_t format = RTC8564::BCD) const;
	uint8_t days(uint8_t format = RTC8564::BCD) const;
	uint8_t weekdays() const;
	uint8_t months(uint8_t format = RTC8564::BCD) const;
	uint8_t years(uint8_t format = RTC8564::BCD) const;
	bool century() const;
};

extern RTC8564 Rtc;

#endif
