/*
  Direct8x8.h - Arduino library for 8x8 LED Dot Matrix.
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef DIRECT8X8_H
#define DIRECT8X8_H

#include <inttypes.h>
#include <avr/pgmspace.h>

class Sprite;

class Direct8x8
{
private:
	uint8_t _rowPins[8];
	uint8_t _colPins[8];
	uint8_t _buffer[8];
	uint8_t _row;
	
public:
	Direct8x8(uint8_t r0,uint8_t r1,uint8_t r2,uint8_t r3,
			  uint8_t r4,uint8_t r5,uint8_t r6,uint8_t r7,
			  uint8_t c0,uint8_t c1,uint8_t c2,uint8_t c3,
			  uint8_t c4,uint8_t c5,uint8_t c6,uint8_t c7);
	
	void write(uint8_t x, uint8_t y, uint8_t value);
	void write(uint8_t x, uint8_t y, const Sprite &sprite);
	void clear(void);
	void hsync(void);
	bool vsync(void);
};

#endif	// Direct8x8.h
