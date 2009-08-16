/*
  Timeline.cpp - Arduino library for Purse Train Timeline Table.
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */
#include <HardwareSerial.h>
#include "Timeline.h"

Timeline::Timeline(const prog_uint8_t *data)
{
	_position = 0;
	_data = data;
	_hours = _minutes = _dest = 0;
}

void Timeline::firstTrain(void)
{
	_position = 0;
	_hours = pgm_read_byte(_data+_position); _position++;
	if(_hours >= 24) _hours -= 24;
	_dest = pgm_read_byte(_data+_position); _position++;
	_minutes = pgm_read_byte(_data+_position); _position++;
}

bool Timeline::nextTrain(void)
{
	if(pgm_read_byte(_data+_position) == END){
		_hours = _minutes = _dest = END;
		return false;
	}else{
		if(pgm_read_byte(_data+_position) == NXT){
			_position++;
			_hours = pgm_read_byte(_data+_position); _position++;
			if(_hours >= 24) _hours -= 24;
		}
		_dest = pgm_read_byte(_data+_position); _position++;
		_minutes = pgm_read_byte(_data+_position); _position++;
	}
	return true;
}

bool Timeline::trainAt(uint8_t hours,uint8_t minutes)
{
	int since,last_p;
	uint8_t last_h,last_m,last_d;
	
	since = ((hours * 60) + minutes);
	
 	firstTrain();
	last_h = _hours;
	last_m = _minutes;
	last_d = _dest;
	last_p = _position;
	
	do{
		if((_hours > hours) || (_hours == hours && _minutes >= minutes)){
			int now = ((_hours * 60) + _minutes) - since;
			int last = ((last_h * 60) + last_m) - since;
			if(last < 0) last = -last;
			
			Serial.print(now);
			Serial.print(" ");
			Serial.print(_hours,DEC);
			Serial.print(":");
			Serial.print(_minutes,DEC);
			Serial.print(" ");
			Serial.print(last);
			Serial.print(" ");
			Serial.print(last_h,DEC);
			Serial.print(":");
			Serial.println(last_m,DEC);
			
			if(now < last){
				last_h = _hours;
				last_m = _minutes;
				last_d = _dest;
				last_p = _position;
			}
		}
	}while(nextTrain());
	_hours = last_h;
	_minutes = last_m;
	_dest = last_d;
	_position = last_p;
	
//	firstTrain();
// 	while((_hours<hours) || (_hours == hours && _minutes >= minutes)){
// 		if(nextTrain() == false)
// 			return false;
// 	}
	return true;
}

uint8_t Timeline::hours() const
{
	return _hours;
}

uint8_t Timeline::minutes() const
{
	return _minutes;
}

uint8_t Timeline::destination() const
{
	return _dest;
}
