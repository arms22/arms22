/*
  Direct8x8.cpp - Arduino library for 8x8 LED Dot Matrix.
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#include <WConstants.h>
#include <pins_arduino.h>
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
	tmp = _buffer[y];
	tmp = tmp & ~(0x01 << x);
	tmp = tmp | ((value & 0x01) << x);
	_buffer[y] = tmp;
}

void Direct8x8::write(uint8_t y, uint8_t value)
{
	_buffer[y] = value;
}

void Direct8x8::clear(void)
{
	uint8_t i;
	for(i=0;i<8;i++){
		_buffer[i] = 0;
	}
	_row = 8;
}

void Direct8x8::turnOff(void)
{
	digitalWrite(_rowPins[_row], LOW);
}

void Direct8x8::turnOn(void)
{
	digitalWrite(_rowPins[_row], HIGH);
}

void Direct8x8::updateRow(void)
{
	_row = (_row + 1) & 7;
	uint8_t rowData = _buffer[_row];
	digitalWrite(_colPins[0], !(rowData & 0x01));
	digitalWrite(_colPins[1], !(rowData & 0x02));
	digitalWrite(_colPins[2], !(rowData & 0x04));
	digitalWrite(_colPins[3], !(rowData & 0x08));
	digitalWrite(_colPins[4], !(rowData & 0x10));
	digitalWrite(_colPins[5], !(rowData & 0x20));
	digitalWrite(_colPins[6], !(rowData & 0x40));
	digitalWrite(_colPins[7], !(rowData & 0x80));
}

bool Direct8x8::vsync(void)
{
	unsigned long t = millis();
	if(t != _lastmillis){
		turnOff();
		delayMicroseconds(100);
		updateRow();
		turnOn();
		_lastmillis = t;
	}
	return (_row == 7);
}
