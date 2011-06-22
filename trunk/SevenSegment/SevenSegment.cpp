#include <WProgram.h>
#include "SevenSegment.h"

const uint8_t SevenSegment::Font[] = {
	/*0*/ SEG_A_BIT | SEG_B_BIT | SEG_C_BIT | SEG_D_BIT | SEG_E_BIT | SEG_F_BIT,
	/*1*/ SEG_B_BIT | SEG_C_BIT,
	/*2*/ SEG_A_BIT | SEG_B_BIT | SEG_D_BIT | SEG_E_BIT | SEG_G_BIT,
	/*3*/ SEG_A_BIT | SEG_B_BIT | SEG_C_BIT | SEG_D_BIT | SEG_G_BIT,
	/*4*/ SEG_B_BIT | SEG_C_BIT | SEG_F_BIT | SEG_G_BIT,
	/*5*/ SEG_A_BIT | SEG_C_BIT | SEG_D_BIT | SEG_F_BIT | SEG_G_BIT,
	/*6*/ SEG_A_BIT | SEG_C_BIT | SEG_D_BIT | SEG_E_BIT | SEG_F_BIT | SEG_G_BIT,
	/*7*/ SEG_A_BIT | SEG_B_BIT | SEG_C_BIT,
	/*8*/ SEG_A_BIT | SEG_B_BIT | SEG_C_BIT | SEG_D_BIT | SEG_E_BIT | SEG_F_BIT | SEG_G_BIT,
	/*9*/ SEG_A_BIT | SEG_B_BIT | SEG_C_BIT | SEG_D_BIT | SEG_F_BIT | SEG_G_BIT,
};

SevenSegment::SevenSegment(uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4,
						   uint8_t segA, uint8_t segB, uint8_t segC, uint8_t segD,
						   uint8_t segE, uint8_t segF, uint8_t segG, uint8_t segDP)
{
	_digitPins[0] = dig1;	_digitPins[1] = dig2;
	_digitPins[2] = dig3;	_digitPins[3] = dig4;

	_segmentPins[0] = segA;	_segmentPins[1] = segB;
	_segmentPins[2] = segC;	_segmentPins[3] = segD;
	_segmentPins[4] = segE;	_segmentPins[5] = segF;
	_segmentPins[6] = segG;	_segmentPins[7] = segDP;

	_numberOfDigits	  = 4;
	_numberOfSegments = 8;
	_updateInterval	  = 3333;
}

void SevenSegment::begin(void)
{
	uint8_t i;

	for(i=0; i<_numberOfDigits; i++){
		pinMode(_digitPins[i], OUTPUT);
		digitalWrite(_digitPins[i], LOW);
	}

	for(i=0; i<_numberOfSegments; i++){
		pinMode(_segmentPins[i], OUTPUT);
		digitalWrite(_segmentPins[i], LOW);
	}

	_currentDigit = 0;
	_lastUpdateTime = 0;
}

void SevenSegment::digitOn(void)
{
	digitalWrite(_digitPins[_currentDigit], LOW);
}

void SevenSegment::digitOff(void)
{
	digitalWrite(_digitPins[_currentDigit], HIGH);
}

void SevenSegment::updateDigit(void)
{
	uint8_t data,mask,i;
	data = _numericData[_currentDigit];
	mask = 1;
	for(i=0; i<_numberOfSegments; i++){
		digitalWrite(_segmentPins[i], data & mask);
		data >>= 1;
	}
}

void SevenSegment::write(uint8_t digit,uint8_t value)
{
	_numericData[digit] = value;
}

void SevenSegment::setDigit(uint8_t digit,uint8_t number)
{
	_numericData[digit] = SevenSegment::Font[number & 15];
}

void SevenSegment::setDp(uint8_t digit,bool value)
{
	if( value ){
		_numericData[digit] |= SEG_DP_BIT;
	}else{
		_numericData[digit] &= ~SEG_DP_BIT;
	}
}

void SevenSegment::print(int value)
{
	uint8_t digit = _numberOfDigits;
	uint8_t sign;

	if(value < 0){
		value = -value;
		sign = -1;
	}else{
		sign = 1;
	}

	do {
		digit--;
		_numericData[digit] = SevenSegment::Font[(value % 10) & 15];
		//setDigit(digit, value % 10);
		value /= 10;
	}while((digit > 0) && (value > 0));

	if(sign < 0){
		_numericData[digit] = SEG_G_BIT;
		//write(digit, SEG_G_BIT); // '-'
	}
}

bool SevenSegment::update(void)
{
	unsigned long cur = micros();
	uint16_t diff = cur - _lastUpdateTime;
	if(diff > _updateInterval){
		digitOff();
		_currentDigit++;
		if(_currentDigit >= _numberOfDigits)
			_currentDigit = 0;
		updateDigit();
		digitOn();
		_lastUpdateTime = micros();
		return _currentDigit == (_numberOfDigits-1);
	}
	return false;
}

void SevenSegment::updateWithDelay(unsigned long ms)
{
	uint16_t start = (uint16_t)micros();
	while (ms > 0) {
		update();
		if (((uint16_t)micros() - start) >= 1000) {
			ms--;
			start += 1000;
		}
	}
}
