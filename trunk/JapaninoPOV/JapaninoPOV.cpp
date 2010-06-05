/*
  JapaninoPOV.cpp
  Copyright (c) arms22 2010. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#include <WProgram.h>
#include <WConstants.h>
#include "JapaninoPOV.h"

#define INVALID_PIN			(0xff)
#define FR_BLANK			(4)
#define BK_BLANK			(2)
#define STAT_WAI_SYN		(0)
#define STAT_FR_BLANK		(1)
#define STAT_DSP_DAT		(2)
#define STAT_BK_BLANK		(3)

JapaninoPOV::JapaninoPOV(uint8_t msbPin)
{
	init(msbPin+6,msbPin+5,msbPin+4,msbPin+3,msbPin+2,msbPin+1,msbPin+0,INVALID_PIN);
}

JapaninoPOV::JapaninoPOV(uint8_t msbPin,uint8_t syncPin)
{
	init(msbPin+6,msbPin+5,msbPin+4,msbPin+3,msbPin+2,msbPin+1,msbPin+0,syncPin);
}

// JapaninoPOV::JapaninoPOV(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3,
// 						 uint8_t d4,uint8_t d5,uint8_t d6)
// {
// 	init(d0,d1,d2,d3,d4,d5,d6,INVALID_PIN);
// }

void JapaninoPOV::init(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3,
					   uint8_t d4,uint8_t d5,uint8_t d6,uint8_t syncPin)
{
	_ledPins[0]		= d0;
	_ledPins[1]		= d1;
	_ledPins[2]		= d2;
	_ledPins[3]		= d3;
	_ledPins[4]		= d4;
	_ledPins[5]		= d5;
	_ledPins[6]		= d6;
	_syncPin		= syncPin;
	_displayWidth	= 20;
	//_frameTime      = 40;
	_updateInterval = 2000;
	_buffer			= NULL;
	_size			= 0;
	_flags.frameTimeAutoDetect = false;
	_flags.scrolling		   = false;
}

void JapaninoPOV::begin(void)
{
	for (uint8_t i=0; i<JAPANINO_POV_NUM_OF_LED; i++) {
		pinMode(_ledPins[i],OUTPUT);
		digitalWrite(_ledPins[i],LOW);
	}
	if (_syncPin != INVALID_PIN) {
		pinMode(_syncPin, INPUT);
		digitalWrite(_syncPin, HIGH);
		_displayStat = STAT_WAI_SYN;
	}
	else{
		_displayStat = STAT_FR_BLANK;
		_dotCount = 0;
	}
	_lastUpdateTime = 0;
	_lastSyncTime = 0;
	_scrollOffset = 0;
}

void JapaninoPOV::write_data(uint8_t data)
{
	digitalWrite(_ledPins[0], data & 0x01);
	digitalWrite(_ledPins[1], data & 0x02);
	digitalWrite(_ledPins[2], data & 0x04);
	digitalWrite(_ledPins[3], data & 0x08);
	digitalWrite(_ledPins[4], data & 0x10);
	digitalWrite(_ledPins[5], data & 0x20);
	digitalWrite(_ledPins[6], data & 0x40);
}

bool JapaninoPOV::update(void)
{
	bool framesync = false;
	
	if(_size){
		unsigned long curtime = micros();
		
		if(_displayStat == STAT_WAI_SYN){
			if(digitalRead(_syncPin) == LOW){
				_displayStat = STAT_FR_BLANK;
				_dotCount = 0;
				if(_flags.frameTimeAutoDetect){
					unsigned long diff = (millis() - _lastSyncTime) / 2;
					if(diff > 255) diff = 255;
					setFrameTime(diff);
					_lastSyncTime = millis();
				}
				goto sync_detected;
			}
		}
		
		if((curtime - _lastUpdateTime) > _updateInterval){
		sync_detected:
			
			if(_displayStat == STAT_FR_BLANK){
				write_data(0x00);
				_dotCount++;
				if(_dotCount >= FR_BLANK){
					_displayStat = STAT_DSP_DAT;
					_dotCount = 0;
					framesync = true;
					if(_flags.scrolling){
						_scrollOffset++;
						if(_scrollOffset >= _size + _displayWidth){
							_scrollOffset = 0;
						}
					}
				}
			}
			else if(_displayStat == STAT_DSP_DAT){
				int i;
				if(_flags.scrolling){
					i = _scrollOffset + _dotCount - _displayWidth;
				}else{
					i = _dotCount;
				}
				if(0 <= i && i < _size){
					write_data(_buffer[i]);
				}else{
					write_data(0x00);
				}
				_dotCount++;
				if(_dotCount >= _displayWidth){
					_displayStat = STAT_BK_BLANK;
					_dotCount = 0;
				}
			}
			else if(_displayStat == STAT_BK_BLANK){
				write_data(0x00);
				_dotCount++;
				if(_dotCount >= BK_BLANK){
					_displayStat = (_syncPin == INVALID_PIN ? STAT_FR_BLANK : STAT_WAI_SYN);
					_dotCount = 0;
				}
			}
			
			_lastUpdateTime = curtime;
		}
	}
	return framesync;
}

// void JapaninoPOV::setPinSync(uint8_t syncPin)
// {
// 	_syncPin = syncPin;
// 	if(syncPin == INVALID_PIN){
// 		if(_displayStat == STAT_WAI_SYN){
// 			_displayStat = STAT_FR_BLANK;
// 			_dotCount = 0;
// 		}
// 	}
// }

void JapaninoPOV::setWidth(uint16_t width)
{
	_displayWidth = width;
	//setFrameTime(_frameTime);
}

void JapaninoPOV::setFrameBuffer(uint8_t *buffer,uint16_t size)
{
	_buffer = buffer;
	_size = size;
}

void JapaninoPOV::setFrameTime(unsigned long ms)
{
	//_frameTime = ms;
	_updateInterval = (ms * 1000) / (_displayWidth + FR_BLANK + BK_BLANK);
}

void JapaninoPOV::setFrameTimeAutoDetect(bool flag)
{
	_flags.frameTimeAutoDetect = flag;
}

void JapaninoPOV::setScrolling(bool flag)
{
	_flags.scrolling = flag;
	_scrollOffset = 0;
}
