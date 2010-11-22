/*
  Dots.cpp - Arduino library for 8x8/5x7 LED Dot Matrix.
  Copyright 2010 arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#include <WConstants.h>
#include <pins_arduino.h>
#include "Dots.h"

Dots::Dots(uint8_t r0,uint8_t r1,uint8_t r2,uint8_t r3,
		   uint8_t r4,uint8_t r5,uint8_t r6,uint8_t r7,
		   uint8_t c0,uint8_t c1,uint8_t c2,uint8_t c3,
		   uint8_t c4,uint8_t c5,uint8_t c6,uint8_t c7)
{
	_rowPins[0] = r0; _rowPins[1] = r1; _rowPins[2] = r2; _rowPins[3] = r3;
	_rowPins[4] = r4; _rowPins[5] = r5; _rowPins[6] = r6; _rowPins[7] = r7;
	
	_colPins[0] = c0; _colPins[1] = c1; _colPins[2] = c2; _colPins[3] = c3;
	_colPins[4] = c4; _colPins[5] = c5; _colPins[6] = c6; _colPins[7] = c7;
	
	_numOfRows = 8;
	_numOfCols = 8;
	_updateInterval = (8333 / 8);
}

Dots::Dots(uint8_t r0,uint8_t r1,uint8_t r2,uint8_t r3,
		   uint8_t r4,uint8_t r5,uint8_t r6,
		   uint8_t c0,uint8_t c1,uint8_t c2,uint8_t c3,
		   uint8_t c4)
{
	_rowPins[0] = r0; _rowPins[1] = r1; _rowPins[2] = r2; _rowPins[3] = r3;
	_rowPins[4] = r4; _rowPins[5] = r5; _rowPins[6] = r6;
	
	_colPins[0] = c0; _colPins[1] = c1; _colPins[2] = c2; _colPins[3] = c3;
	_colPins[4] = c4;
	
	_numOfRows = 7;
	_numOfCols = 5;
	_updateInterval = (8333 / 7);
}

void Dots::begin(void)
{
	uint8_t i;
	for(i=0;i<_numOfRows;i++){
		pinMode(_rowPins[i], OUTPUT);
	}
	for(i=0;i<_numOfCols;i++){
		pinMode(_colPins[i], OUTPUT);
	}
	clear();
}

void Dots::write(uint8_t x, uint8_t y, uint8_t value)
{
	uint8_t tmp,msk;
	tmp = _buffer[y];
	msk = 0x80 >> x;
	tmp = tmp & ~msk;
	tmp = tmp | (value ? msk : 0);
	_buffer[y] = tmp;
}

void Dots::write(uint8_t y, uint8_t value)
{
	_buffer[y] = value;
}

void Dots::write(uint8_t y, const uint8_t values[], uint8_t size)
{
	uint8_t i;
	for(i=0;i<size;i++)
		_buffer[(y++) & 0x07] = values[i];
}

void Dots::clear(void)
{
	uint8_t i;
	for(i=0;i<8;i++){
		_buffer[i] = 0;
	}
	_row = _numOfRows - 1;
}

void Dots::turnOff(void)
{
	digitalWrite(_rowPins[_row], LOW);
}

void Dots::turnOn(void)
{
	digitalWrite(_rowPins[_row], HIGH);
}

void Dots::updateRow(void)
{
	uint8_t i,data,mask;
	_row++;
	if(_row >= _numOfRows){
		_row = 0;
	}
	data = _buffer[_row];
	mask = 0x80;
	for(i=0;i<_numOfCols;i++){
		if(data & mask){
			digitalWrite(_colPins[i], LOW);
		}else{
			digitalWrite(_colPins[i], HIGH);
		}
		mask >>= 1;
	}
}

bool Dots::update(void)
{
	unsigned long t = micros();
	bool sync = false;
	if((t - _lastUpdateTime) > _updateInterval){
		turnOff();
		updateRow();
		turnOn();
		_lastUpdateTime = t;
		sync = (_row == (_numOfRows-1));
	}
	return sync;
}

void Dots::updateWithDelay(unsigned long ms)
{
	unsigned long start = millis();
	do{
		bool sync = update();
		if(sync){
			unsigned long t = millis();
			if((t - start) >= ms){
				break;
			}
		}
	}while(1);
}
