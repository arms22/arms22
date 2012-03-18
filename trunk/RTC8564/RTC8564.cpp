#include "RTC8564.h"
#include <Wire.h>

#define RTC8564_SLAVE_ADRS	(0xA2 >> 1)
#define BCD2Decimal(x)		(((x>>4)*10)+(x&0xf))

RTC8564 Rtc = RTC8564();

RTC8564::RTC8564()
	: _seconds(0), _minutes(0), _hours(0), _days(0), _weekdays(0), _months(0), _years(0), _century(0)
{
}

void RTC8564::init(void)
{
	delay(1000);
	Wire.beginTransmission(RTC8564_SLAVE_ADRS);
	Wire.write(byte(0x00));			// write reg addr 00
	Wire.write(byte(0x20));			// 00 Control 1, STOP=1
	Wire.write(byte(0x00));			// 01 Control 2
	Wire.write(byte(0x00));			// 02 Seconds
	Wire.write(byte(0x00));			// 03 Minutes
	Wire.write(byte(0x09));			// 04 Hours
	Wire.write(byte(0x01));			// 05 Days
	Wire.write(byte(0x01));			// 06 Weekdays
	Wire.write(byte(0x01));			// 07 Months
	Wire.write(byte(0x01));			// 08 Years
	Wire.write(byte(0x00));			// 09 Minutes Alarm
	Wire.write(byte(0x00));			// 0A Hours Alarm
	Wire.write(byte(0x00));			// 0B Days Alarm
	Wire.write(byte(0x00));			// 0C Weekdays Alarm
	Wire.write(byte(0x00));			// 0D CLKOUT
	Wire.write(byte(0x00));			// 0E Timer control
	Wire.write(byte(0x00));			// 0F Timer
	Wire.write(byte(0x00));			// 00 Control 1, STOP=0
	Wire.endTransmission();
}

void RTC8564::begin(void)
{
	Wire.begin();
	if(isvalid() == false)
		init();
}

void RTC8564::sync(uint8_t date_time[],uint8_t size)
{
	Wire.beginTransmission(RTC8564_SLAVE_ADRS);
	Wire.write(byte(0x00));			// write reg addr 00
	Wire.write(byte(0x20));			// 00 Control 1, STOP=1
	Wire.endTransmission();
	
	Wire.beginTransmission(RTC8564_SLAVE_ADRS);
	Wire.write(byte(0x02));			// write reg addr 02
	Wire.write(date_time, size);
	Wire.endTransmission();
	
	Wire.beginTransmission(RTC8564_SLAVE_ADRS);
	Wire.write(byte(0x00));			// write reg addr 00
	Wire.write(byte(0x00));			// 00 Control 1, STOP=0
	Wire.endTransmission();
}

bool RTC8564::available(void)
{
	uint8_t buff[7];
	
	Wire.beginTransmission(RTC8564_SLAVE_ADRS);
	Wire.write(byte(0x02));			// write reg addr 02
	Wire.endTransmission();
	
	Wire.requestFrom(RTC8564_SLAVE_ADRS, 7);
	
	for(int i=0; i<7; i++){
		if(Wire.available()){
			buff[i] = Wire.read();
		}
	}
	
	_seconds  = buff[0] & 0x7f;
	_minutes  = buff[1] & 0x7f;
	_hours	  = buff[2] & 0x3f;
	_days	  = buff[3] & 0x3f;
	_weekdays = buff[4] & 0x07;
	_months	  = buff[5] & 0x1f;
	_years	  = buff[6];
	_century  = (buff[5] & 0x80) ? 1 : 0;
	return (buff[0] & 0x80 ? false : true);
}

bool RTC8564::isvalid(void)
{
	Wire.beginTransmission(RTC8564_SLAVE_ADRS);
	Wire.write(byte(0x02));			// write reg addr 02
	Wire.endTransmission();
	Wire.requestFrom(RTC8564_SLAVE_ADRS, 1);
	if(Wire.available()){
		uint8_t buff = Wire.read();
		return (buff & 0x80 ? false : true);
	}
	return false;
}

uint8_t RTC8564::seconds(uint8_t format) const {
	if(format == Decimal) return BCD2Decimal(_seconds);
	return _seconds;
}

uint8_t RTC8564::minutes(uint8_t format) const {
	if(format == Decimal) return BCD2Decimal(_minutes);
	return _minutes;
}

uint8_t RTC8564::hours(uint8_t format) const {
	if(format == Decimal) return BCD2Decimal(_hours);
	return _hours;
}

uint8_t RTC8564::days(uint8_t format) const {
	if(format == Decimal) return BCD2Decimal(_days);
	return _days;
}

uint8_t RTC8564::weekdays() const {
	return _weekdays;
}

uint8_t RTC8564::months(uint8_t format) const {
	if(format == Decimal) return BCD2Decimal(_months);
	return _months;
}

uint8_t RTC8564::years(uint8_t format) const {
	if(format == Decimal) return BCD2Decimal(_years);
	return _years;
}

bool RTC8564::century() const {
	return _century;
}
