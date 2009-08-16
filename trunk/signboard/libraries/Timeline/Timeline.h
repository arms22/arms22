/*
  Timeline.h - Arduino library for Purse Train Timeline Table.
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */
#ifndef TIMELINE_H
#define TIMELINE_H

#include <inttypes.h>
#include <avr/pgmspace.h>

class Timeline
{
private:
	int _position;
	uint8_t _hours;
	uint8_t _minutes;
	uint8_t _dest;
	const prog_uint8_t *_data;
public:
	enum {
		NXT = 0xfe,
		END = 0xff,
	};
	Timeline(const prog_uint8_t *data);
	void firstTrain(void);
	bool nextTrain(void);
	bool trainAt(uint8_t hour,uint8_t minute);
	uint8_t hours() const;
	uint8_t minutes() const;
	uint8_t destination() const;
};

#endif
