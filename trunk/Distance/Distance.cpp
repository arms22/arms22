/*
  Distance.h - Arduino library for Distance Measuring Sensor Unit(SHARP GP2Y0A21YK0F)
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#include <WConstants.h>
#include "Distance.h"


Distance::Distance(uint8_t analogIn)
	: _analogIn(analogIn)
	, _analogValue(0)
{
}

int Distance::available(void)
{
	_analogValue = (_analogValue - (_analogValue >> kLowPassFilter)) + (analogRead(_analogIn) >> kLowPassFilter);
	return _analogValue;
}

int Distance::analogValue(uint8_t linearize)
{
	if(linearize){
		int value;
		if(_analogValue > 328){
			value = (_analogValue-328)*(0.25)+573;
		}else if(_analogValue > 205){
			value = (_analogValue-205)*(0.67)+491;
		}else if(_analogValue > 123){
			value = (_analogValue-123)*(2.25)+307;
		}else{
			value = (_analogValue-81)*(5.50)+81;
		}
		return value;
	}
	return _analogValue;
}

int Distance::numberOfDistance(void)
{
	int distance;
	
	if(_analogValue > 328){
		distance = (_analogValue-328)*(-0.31)+160;
	}else if(_analogValue > 205){
		distance = (_analogValue-205)*(-0.9)+270;
	}else if(_analogValue > 123){
		distance = (_analogValue-123)*(-2.81)+500;
	}else{
		distance = (_analogValue-81)*(-7.32)+800;
	}
	
	return distance;
}
