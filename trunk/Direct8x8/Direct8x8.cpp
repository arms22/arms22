/*
  Direct8x8.cpp - Arduino library for 8x8 LED Dot Matrix.
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#include <WConstants.h>
#include <pins_arduino.h>
#include <Sprite.h>
#include "Direct8x8.h"

Direct8x8::Direct8x8(uint8_t r0,uint8_t r1,uint8_t r2,uint8_t r3,
					 uint8_t r4,uint8_t r5,uint8_t r6,uint8_t r7,
					 uint8_t c0,uint8_t c1,uint8_t c2,uint8_t c3,
					 uint8_t c4,uint8_t c5,uint8_t c6,uint8_t c7)
{
	_rowPins[0] = r0; _rowPins[1] = r1; _rowPins[2] = r2; _rowPins[3] = r3;
	_rowPins[4] = r4; _rowPins[5] = r5; _rowPins[6] = r6; _rowPins[7] = r7;
	
	_colPins[0] = c0; _colPins[1] = c1; _colPins[2] = c2; _colPins[3] = c3;
	_colPins[4] = c4; _colPins[5] = c5; _colPins[6] = c6; _colPins[7] = c7;
	
	uint8_t i;
	for(i=0;i<8;i++){
		pinMode(_rowPins[i], OUTPUT);
	}
	for(i=0;i<8;i++){
		pinMode(_colPins[i], OUTPUT);
	}
	
	clear();
}

void Direct8x8::write(uint8_t x, uint8_t y, uint8_t value)
{
	uint8_t tmp;
//	y &= 7;
	tmp = _buffer[y];
//	x &= 7;
	tmp = tmp & ~(0x01 << x);
	tmp = tmp | ((value & 0x01) << x);
	_buffer[y] = tmp;
}

void Direct8x8::write(uint8_t x, uint8_t y, const Sprite &sprite)
{
	uint8_t w = sprite.width();
	uint8_t h = sprite.height();
	for (uint8_t i=0; i<h; i++)
		for (uint8_t j=0; j<w; j++)
			write(x + j, y + i, sprite.read(j, i));
}

void Direct8x8::clear(void)
{
	uint8_t i;
	for(i=0;i<8;i++){
		_buffer[i] = 0;
	}
	_row = 8;
}

void Direct8x8::hsync(void)
{
	digitalWrite(_rowPins[_row - 1], LOW);
	if(_row < 8)
		;
	else
		_row = 0;
	uint8_t col,rowData = _buffer[_row];
	for(col=0;col<8;col++){
		digitalWrite(_colPins[col], !((rowData >> col) & 0x01));
	}
	digitalWrite(_rowPins[_row], HIGH);
	_row++;
}

bool Direct8x8::vsync(void)
{
	hsync();
	delayMicroseconds(950);
	return (_row == 8);
}
