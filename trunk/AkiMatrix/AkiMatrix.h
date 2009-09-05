/*
  AkiMatrix.h - Arduino library for akizuki LED Dot-Matrix unit.
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */

#ifndef AKIMATRIX_H
#define AKIMATRIX_H

#include <inttypes.h>
#include <avr/pgmspace.h>

#define AKI_MATRIX_CASCADE	(3)

#define AKI_MATRIX_NUMBER_OF_ROWS	(16)
#define AKI_MATRIX_NUMBER_OF_COLS	(32*AKI_MATRIX_CASCADE)

#define AKI_MATRIX_VRAM_ROWS (AKI_MATRIX_NUMBER_OF_ROWS)
#define AKI_MATRIX_VRAM_COLS (AKI_MATRIX_NUMBER_OF_COLS/8)

#define AKI_MATRIX_DIRECT_IO
#define AKI_MATRIX_tCLOCK	(1)

class AkiMatrix
{
private:
#ifdef AKI_MATRIX_DIRECT_IO
	uint8_t _bit_sin1, _bit_sin2, _bit_sin3, _bit_clock;
	uint8_t _port_sin1,_port_sin2,_port_sin3,_port_clock;
#else
	uint8_t _pinSin1;			/* Row */
	uint8_t _pinSin2;			/* LED1 */
	uint8_t _pinSin3;			/* LED2 */
	uint8_t _pinClock;
#endif
	uint8_t _pinLatch;
	uint8_t _pinStrob;
	
	uint8_t _vramBuffer[AKI_MATRIX_VRAM_ROWS][AKI_MATRIX_VRAM_COLS];
	uint8_t _currentScanRow;
	
	void shiftOut(uint16_t sin1,uint16_t sin2,uint16_t sin3);
	
public:
	AkiMatrix(uint8_t sin1,uint8_t sin2,uint8_t sin3,uint8_t clock,uint8_t latch,uint8_t strob);
	void bitBlt(uint8_t x,uint8_t y,const uint8_t *src,uint8_t w,uint8_t h);
	void bitBlt_P(uint8_t x,uint8_t y,const prog_uint8_t *src,uint8_t w,uint8_t h);
	void write(uint8_t x, uint8_t y, uint8_t value);
	void write(uint8_t x, uint8_t y, uint8_t w,uint8_t h,const uint8_t *src);
	void clear(void);
	void hsync(void);
	bool vsync(void);
	void setOutputEnable(uint8_t flag);
};

#endif	// AkiMatrix.h
