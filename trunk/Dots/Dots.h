/*
  Dots.h - Arduino library for 8x8/5x7 Dot LED Matrix.
  Copyright 2010 arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef DOTS_H
#define DOTS_H

#include <inttypes.h>

#define Dotsduino_12c (1)
#define Dotsduino_12d (2)

class Dots
{
private:
	uint8_t _rowPins[8];
	uint8_t _colPins[8];
	uint8_t _buffer[8];
	uint8_t _numOfRows;
	uint8_t _numOfCols;
	uint8_t _row;
	uint8_t _autoDetect;
	void init12d(void);
	void init12c(void);
	void autoDetect(void);
public:
	Dots(int which = 0);
	
	Dots(uint8_t r0,uint8_t r1,uint8_t r2,uint8_t r3,
		 uint8_t r4,uint8_t r5,uint8_t r6,uint8_t r7,
		 uint8_t c0,uint8_t c1,uint8_t c2,uint8_t c3,
		 uint8_t c4,uint8_t c5,uint8_t c6,uint8_t c7);
	
	Dots(uint8_t r0,uint8_t r1,uint8_t r2,uint8_t r3,
		 uint8_t r4,uint8_t r5,uint8_t r6,
		 uint8_t c0,uint8_t c1,uint8_t c2,uint8_t c3,
		 uint8_t c4);
	
	void begin(void);
	void end(void);
	void write(uint8_t x, uint8_t y, uint8_t value);
	void write(uint8_t y, uint8_t value);
	void write(uint8_t y, const uint8_t values[], uint8_t size);
	void clear(void);
	void update(void);
	static Dots *active_object;
};

#endif	// Dots.h
