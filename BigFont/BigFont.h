/*
  BigFont.h - Big Font LCD library for Arduino
  Copyright 2011(c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#ifndef BigFont_H_
#define BigFont_H_

#include <LiquidCrystal.h>

class BigFont : public Print
{
public:
	BigFont();
	~BigFont();
	void attach(LiquidCrystal *lcd);
	void clear();
	void home();
	void setInvert(bool yes);
	void setCursor(uint8_t col, uint8_t row);
	virtual size_t write(uint8_t);
private:
	uint8_t _cur_col;
	uint8_t _cur_row;
};

#endif
