/*
  AkiMatrix.cpp - Arduino library for akizuki LED Dot-Matrix unit.
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#include <WConstants.h>
#include <pins_arduino.h>
#include "AkiMatrix.h"

AkiMatrix::AkiMatrix(uint8_t sin1,uint8_t sin2,uint8_t sin3,uint8_t clock,uint8_t latch,uint8_t strob)
{
	pinMode(sin1, OUTPUT);
	pinMode(sin2, OUTPUT);
	pinMode(sin3, OUTPUT);
	pinMode(clock, OUTPUT);
	pinMode(latch, OUTPUT);
	pinMode(strob, OUTPUT);
	
	digitalWrite(sin1, LOW);
	digitalWrite(sin2, LOW);
	digitalWrite(sin3, LOW);
	digitalWrite(clock, LOW);
	digitalWrite(latch, HIGH);	// data hold(inverted)
	digitalWrite(strob, LOW);	// output enable
	
#ifdef AKI_MATRIX_DIRECT_IO
	_bit_sin1 = digitalPinToBitMask(sin1);
	_bit_sin2 = digitalPinToBitMask(sin2);
	_bit_sin3 = digitalPinToBitMask(sin3);
	_bit_clock = digitalPinToBitMask(clock);
	_port_sin1 = digitalPinToPort(sin1);
	_port_sin2 = digitalPinToPort(sin2);
	_port_sin3 = digitalPinToPort(sin3);
	_port_clock = digitalPinToPort(clock);
#else
	_pinSin1 = sin1;
	_pinSin2 = sin2;
	_pinSin3 = sin3;
	_pinClock = clock;
#endif
	_pinLatch = latch;
	_pinStrob = strob;
	
	clear();
}

void AkiMatrix::bitBlt(uint8_t x,uint8_t y,const uint8_t *src,uint8_t w,uint8_t h)
{
	uint8_t vx,vy,i,j;
	vx = x / 8;
	vy = y;
	w = (w + 7) / 8;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			_vramBuffer[vy+i][vx+j] = *src++;
		}
	}
}

void AkiMatrix::bitBlt_P(uint8_t x,uint8_t y,const prog_uint8_t *src,uint8_t w,uint8_t h)
{
	uint8_t vx,vy,i,j;
	vx = x / 8;
	vy = y;
	w = (w + 7) / 8;
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			_vramBuffer[vy+i][vx+j] = pgm_read_byte(src++);
		}
	}
}

void AkiMatrix::write(uint8_t x,uint8_t y,uint8_t value)
{
	uint8_t vx,vy,mask,temp;
	mask = 1 << (x % 8);
	vx = x / 8;
	vy = y;
	temp = _vramBuffer[vy][vx];
	if(value){
		temp |= mask;
	}else{
		temp &= ~mask;
	}
	_vramBuffer[vy][vx] = temp;
}

void AkiMatrix::write(uint8_t x, uint8_t y, uint8_t sw,uint8_t sh,const uint8_t *src)
{
	uint8_t i,j,w,h;
	if((x > AKI_MATRIX_NUMBER_OF_COLS) || (y > AKI_MATRIX_NUMBER_OF_ROWS)){
		return;
	}
 	w = sw;
 	sw = (sw + 7) / 8;
	if((x+w) > AKI_MATRIX_NUMBER_OF_COLS){
		w = AKI_MATRIX_NUMBER_OF_COLS - x;
	}
 	h = sh;
	if((y+h) > AKI_MATRIX_NUMBER_OF_ROWS){
		h = AKI_MATRIX_NUMBER_OF_ROWS - y;
	}
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			write(x+j, y+i,
				  src[(i*sw) + (j/8)] & (1<<(j%8)));
		}
	}
}

void AkiMatrix::clear(void)
{
	uint8_t row,col;
	for(row=0; row<AKI_MATRIX_VRAM_ROWS; row++){
		for(col=0; col<AKI_MATRIX_VRAM_COLS; col++){
			_vramBuffer[row][col] = 0;
		}
	}
	_currentScanRow = 0;
}

void AkiMatrix::shiftOut(uint16_t sin1,uint16_t sin2,uint16_t sin3)
{
	uint8_t i;
	uint16_t mask = 0x8000;
#ifdef AKI_MATRIX_DIRECT_IO
	volatile uint8_t *out_sin1 = portOutputRegister(_port_sin1);
	volatile uint8_t *out_sin2 = portOutputRegister(_port_sin2);
	volatile uint8_t *out_sin3 = portOutputRegister(_port_sin3);
	volatile uint8_t *out_clock = portOutputRegister(_port_clock);
#endif
	for(i=0; i<16; i++){
#ifdef AKI_MATRIX_DIRECT_IO
		if(sin1 & mask){
			*out_sin1 |= _bit_sin1;
		}else{
			*out_sin1 &= ~_bit_sin1;
		}
		if(sin2 & mask){
			*out_sin2 |= _bit_sin2;
		}else{
			*out_sin2 &= ~_bit_sin2;
		}
		if(sin3 & mask){
			*out_sin3 |= _bit_sin3;
		}else{
			*out_sin3 &= ~_bit_sin3;
		}
		*out_clock |= _bit_clock;
		if(AKI_MATRIX_tCLOCK)
			delayMicroseconds(AKI_MATRIX_tCLOCK);
		*out_clock &= ~_bit_clock;
		if(AKI_MATRIX_tCLOCK)
			delayMicroseconds(AKI_MATRIX_tCLOCK);
#else
		if(sin1 & mask){
			digitalWrite(_pinSin1, HIGH);
		}else{
			digitalWrite(_pinSin1, LOW);
		}
		if(sin2 & mask){
			digitalWrite(_pinSin2, HIGH);
		}else{
			digitalWrite(_pinSin2, LOW);
		}
		if(sin3 & mask){
			digitalWrite(_pinSin3, HIGH);
		}else{
			digitalWrite(_pinSin3, LOW);
		}
		digitalWrite(_pinClock, HIGH);
		if(AKI_MATRIX_tCLOCK)
			delayMicroseconds(AKI_MATRIX_tCLOCK);
		digitalWrite(_pinClock, LOW);
		if(AKI_MATRIX_tCLOCK)
			delayMicroseconds(AKI_MATRIX_tCLOCK);
#endif
		mask >>= 1;
	}
#ifdef AKI_MATRIX_DIRECT_IO
	*out_sin1 &= ~_bit_sin1;
	*out_sin2 &= ~_bit_sin2;
	*out_sin3 &= ~_bit_sin3;
	*out_clock &= ~_bit_clock;
#else
	digitalWrite(_pinSin1, LOW);
	digitalWrite(_pinSin2, LOW);
	digitalWrite(_pinSin3, LOW);
	digitalWrite(_pinClock, LOW);
#endif
}

void AkiMatrix::hsync(void)
{
	uint16_t row = 0x8000 >> _currentScanRow;
	
	for(uint8_t i=AKI_MATRIX_VRAM_COLS;i>0;i-=4){
		shiftOut( row,
				  (_vramBuffer[_currentScanRow][i-4])|((_vramBuffer[_currentScanRow][i-3])<<8),
				  (_vramBuffer[_currentScanRow][i-2])|((_vramBuffer[_currentScanRow][i-1])<<8));
	}
	
	digitalWrite(_pinLatch, LOW);
	digitalWrite(_pinLatch, HIGH);
	
	_currentScanRow++;
	if(_currentScanRow >= AKI_MATRIX_VRAM_ROWS){
		_currentScanRow = 0;
	}
}

bool AkiMatrix::vsync(void)
{
	hsync();
	return (_currentScanRow == 0) ? true : false;
}

void AkiMatrix::setOutputEnable(uint8_t flag)
{
	if(flag){
		digitalWrite(_pinStrob, LOW);
	}else{
		digitalWrite(_pinStrob, HIGH);
	}
}
