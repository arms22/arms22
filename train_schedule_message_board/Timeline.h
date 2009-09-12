/*
  Timeline.h - Arduino library for Purse Train Timeline Table.
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */
#ifndef TIMELINE_H
#define TIMELINE_H

#include <stdint.h>
#include <avr/pgmspace.h>

class TimelineClass
{
private:
	int _position;
	uint8_t _hours;
	uint8_t _minutes;
	uint8_t _dest;
	const prog_uint8_t *_data;
public:
	TimelineClass();
	void firstTrain(void);
	bool nextTrain(void);
	bool passed(uint8_t hours,uint8_t minutes);
	bool trainAt(uint8_t hours,uint8_t minutes,uint8_t weekday = 1);
	uint8_t hours() const;
	uint8_t minutes() const;
	uint8_t destination() const;
	const prog_uint16_t *destinationString() const;
};

extern TimelineClass Timeline;

#endif
