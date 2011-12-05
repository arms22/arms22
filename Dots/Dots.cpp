/*
 Dots.cpp - Arduino library for 8x8/5x7 Dot LED Matrix.
 Copyright 2010,2011 arms22. All right reserved.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#include <Arduino.h>
#include <avr/interrupt.h>
#include "Dots.h"

Dots::Dots(int which)
{
	if(which == Dotsduino_12d){
		init12d();
	}
	else if(which == Dotsduino_12c){
		init12c();
	}
	else{
		_autoDetect = true;
	}
}

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
	_autoDetect = false;
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
	_autoDetect = false;
}

void Dots::init12d(void)
{
	_rowPins[0] = 2;	_rowPins[1] = 3;	_rowPins[2] = 4;	_rowPins[3] = 13;
	_rowPins[4] = 14;   _rowPins[5] = 15;   _rowPins[6] = 16;   _rowPins[7] = 17;
	
	_colPins[0] = 12;   _colPins[1] = 11;   _colPins[2] = 10;   _colPins[3] = 9;
	_colPins[4] = 5;	_colPins[5] = 6;	_colPins[6] = 7;	_colPins[7] = 8;
	
	_numOfRows = 8;
	_numOfCols = 8;
	_autoDetect = false;
}

void Dots::init12c(void)
{
	_rowPins[0] = 9;	_rowPins[1] = 4;	_rowPins[2] = 10;   _rowPins[3] = 6;
	_rowPins[4] = 17;   _rowPins[5] = 11;   _rowPins[6] = 16;   _rowPins[7] = 13;
	
	_colPins[0] = 5;	_colPins[1] = 15;   _colPins[2] = 14;   _colPins[3] = 8;
	_colPins[4] = 12;   _colPins[5] = 7;	_colPins[6] = 3;	_colPins[7] = 2;
	
	_numOfRows = 8;
	_numOfCols = 8;
	_autoDetect = false;
}

void Dots::autoDetect(void)
{
	uint8_t i, row, col, cnt;
	
	for(i=2; i<18; i++){
		pinMode(i, INPUT);
		digitalWrite(i, LOW);
	}
	
	row = 9;					// 1.2c: R0 1.2d: C3
	col = 15;					// 1.2c: C1 1.2d: R5
	
	pinMode(row, OUTPUT);
	pinMode(col, OUTPUT);
	
	// discharge
	digitalWrite(col, LOW);
	delayMicroseconds(1000);
	
	// charge
	digitalWrite(col, HIGH);
	delayMicroseconds(10);
	
	// measure
	pinMode(col, INPUT);
	digitalWrite(col, LOW);
	
	for(cnt=0; digitalRead(col) && (cnt < 200); cnt++);
	
	// which matrix?
	if(cnt > 50){
		init12c();
	}else{
		init12d();
	}
}

void Dots::begin(void)
{
	uint8_t i;
	if(_autoDetect){
		autoDetect();
	}
	for(i=0;i<_numOfRows;i++){
		pinMode(_rowPins[i], OUTPUT);
	}
	for(i=0;i<_numOfCols;i++){
		pinMode(_colPins[i], OUTPUT);
	}
	clear();
	Dots::active_object = this;
	_row = 0;
	TCCR0A	= 0;
	OCR0A	= 0;
	TIFR0  |= _BV(OCF0A);
	TIMSK0 |= _BV(OCIE0A);
}

void Dots::end(void)
{
	TIMSK0 &= ~_BV(OCIE0A);
	Dots::active_object = 0;
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
}

void Dots::update(void)
{
	uint8_t i,data,mask;
	digitalWrite(_rowPins[_row], LOW);
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
	digitalWrite(_rowPins[_row], HIGH);
}

Dots *Dots::active_object = 0;

SIGNAL(TIMER0_COMPA_vect)
{
	if(Dots::active_object){
		Dots::active_object->update();
	}
}
