/*
  Distance.h - Arduino library for Distance Measuring Sensor Unit(SHARP GP2Y0A21YK0F)
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef DISTANCE_h
#define DISTANCE_h

#include <inttypes.h>

#define kLowPassFilter 1

class Distance
{
	uint8_t _analogIn;
	int _analogValue;
	
public:
	Distance(uint8_t analogIn);
	int available(void);
	int analogValue(uint8_t linearize = false);
	int numberOfDistance(void);
};

#endif // Distance.h
