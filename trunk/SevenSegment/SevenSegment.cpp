#include <WProgram.h>
#include "SevenSegment.h"

const uint8_t SevenSegment::NUMERICAL_NUMBER[] = {
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
	uint8_t digits[] = {dig1,dig2,dig3,dig4,0xff};
	uint8_t segments[] = {segA,segB,segC,segD,segE,segF,segG,segDP,0xff};
	init(digits,segments);
}

void SevenSegment::init(uint8_t digits[],uint8_t segments[])
{
	uint8_t cnt;
	for(cnt=0; digits[cnt]!=0xff; cnt++){
		_digitPins[cnt] = digits[cnt];
		_numericData[cnt] = 0;
	}
	_numberOfDigits = cnt;
	for(cnt=0; segments[cnt]!=0xff; cnt++){
		_segmentPins[cnt] = segments[cnt];
	}
	_numberOfSegments = cnt;
	//_anodeCommon = 0;
	_updateInterval = (16666 / _numberOfDigits);
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

void SevenSegment::write(uint8_t digit,uint8_t value)
{
	_numericData[digit] = value;
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
	_currentDigit++;
	if(_currentDigit >= _numberOfDigits)
		_currentDigit = 0;
	data = _numericData[_currentDigit];
	mask = 0x1;
	for(i=0; i<_numberOfSegments; i++){
		digitalWrite(_segmentPins[i], data & mask);
		data >>= 1;
	}
}

bool SevenSegment::update(void)
{
	unsigned long cur = micros();
	uint16_t diff = cur - _lastUpdateTime;
	if(diff > _updateInterval){
		digitOff();
		delayMicroseconds(50);
		updateDigit();
		digitOn();
		_lastUpdateTime = cur;
		return _currentDigit == 0;
	}
	return false;
}

void SevenSegment::print(long value)
{
	uint8_t digit = _numberOfDigits;
	do {
		digit--;
		write(digit, SevenSegment::NUMERICAL_NUMBER[value % 10]);
		value /= 10;
	}while(digit > 0);
}
