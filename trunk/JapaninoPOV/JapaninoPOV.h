/*
  JapaninoPOV.h
  Copyright (c) arms22 2010. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef JapaninoPOV_h
#define JapaninoPOV_h

#include <inttypes.h>

#define JAPANINO_POV_NUM_OF_LED  7

class JapaninoPOV
{
public:
	JapaninoPOV(uint8_t msbPin);
	JapaninoPOV(uint8_t msbPin,uint8_t syncPin);
	// 	JapaninoPOV(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3,
	// 				uint8_t d4,uint8_t d5,uint8_t d6);
	void begin(void);
	bool update(void);
	void setPinSync(uint8_t syncPin);
	void setWidth(uint16_t width);
	void setFrameBuffer(uint8_t *buffer,uint16_t size);
	void setFrameTime(unsigned long ms);
	void setFrameTimeAutoDetect(bool flag);
	void setScrolling(bool flag);
private:
	void init(uint8_t d0,uint8_t d1,uint8_t d2,uint8_t d3,
			  uint8_t d4,uint8_t d5,uint8_t d6,uint8_t syncPin);
	void write_data(uint8_t data);
	unsigned long	 _updateInterval; // micros
	unsigned long	 _lastUpdateTime; // micros
	unsigned long	 _lastSyncTime;	  // millis
	// 	unsigned long    _frameTime;	  // millis
	uint8_t			*_buffer;
	uint16_t		 _size;
	uint16_t		 _scrollOffset;
	uint16_t   		 _displayWidth;
	uint8_t			 _ledPins[JAPANINO_POV_NUM_OF_LED];
	uint8_t			 _syncPin;
	uint8_t			 _dotCount;
	uint8_t			 _displayStat;
	struct{
		uint8_t frameTimeAutoDetect :1;
		uint8_t scrolling :1;
	} _flags;
};

#endif
