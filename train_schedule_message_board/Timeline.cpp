/*
  Timeline.cpp - Arduino library for Purse Train Timeline Table.
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */
#include "Timeline.h"
#include "train_schedule.h"

TimelineClass::TimelineClass()
{
	_data = BusinessdayTimeline;
	firstTrain();
}

void TimelineClass::firstTrain(void)
{
	_position = 0;
	_hours = pgm_read_byte(_data+_position); _position++;
	_dest = pgm_read_byte(_data+_position); _position++;
	_minutes = pgm_read_byte(_data+_position); _position++;
}

bool TimelineClass::nextTrain(void)
{
	if(pgm_read_byte(_data+_position) == EOT){
		_hours = _minutes = _dest = 0xff;
		return false;
	}else{
		if(pgm_read_byte(_data+_position) == EOL){
			_position++;
			_hours = pgm_read_byte(_data+_position); _position++;
		}
		_dest = pgm_read_byte(_data+_position); _position++;
		_minutes = pgm_read_byte(_data+_position); _position++;
	}
	return true;
}

bool TimelineClass::passed(uint8_t hours,uint8_t minutes)
{
	int now,dep;
	if(hours < BOUNDARY_OF_SERVICE){
		hours += 24;
	}
	now = minutes + (hours * 60);
	dep = _minutes + (_hours * 60);
	return (now > dep);
}

bool TimelineClass::trainAt(uint8_t hours,uint8_t minutes,uint8_t weekday)
{
	bool ret;
	
	if(hours < BOUNDARY_OF_SERVICE){
		if(weekday > 0){
			weekday--;
		}else{
			weekday = 6;
		}
	}
	
	if(weekday == 0) {
		_data = HolidayTimeline;
	}else if(weekday == 6) {
		_data = SaturdayTimeline;
	}else{
		_data = BusinessdayTimeline;
	}
	
	firstTrain();
	do{
		if(passed(hours,minutes))
			;
		else
			break;
	}while((ret = nextTrain()));
	
	return ret;
}

uint8_t TimelineClass::hours() const
{
	return _hours;
}

uint8_t TimelineClass::minutes() const
{
	return _minutes;
}

uint8_t TimelineClass::destination() const
{
	return _dest;
}

const prog_uint16_t *TimelineClass::destinationString() const
{
	return (const prog_uint16_t*)pgm_read_word(&DestinationStrings[_dest]);
}

/////////////////////////////////////////////////////////////////////////////
TimelineClass Timeline = TimelineClass();
