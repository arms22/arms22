/*
  BigFont.cpp - Big Font LCD library for Arduino
  Copyright 2011(c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
*/

#include "BigFont.h"
#include "utility/phi_big_font.h"

BigFont::BigFont()
{
	_cur_col = _cur_row = 0;
}

BigFont::~BigFont()
{
}

void BigFont::attach(LiquidCrystal *lcd)
{
	init_big_font(lcd);
	invert_big_font(false);
}

// void BigFont::detach()
// {
// }

void BigFont::clear()
{
	lcd_clear();
	home();
}

void BigFont::home()
{
	_cur_col = _cur_row = 0;
}

void BigFont::setInvert(bool yes)
{
	invert_big_font(yes);
}

void BigFont::setCursor(uint8_t col, uint8_t row)
{
	_cur_col = col;
	_cur_row = row;
}

size_t BigFont::write(uint8_t ch)
{
	render_big_char(ch, _cur_col, _cur_row);
	_cur_col += 4;
    return 1;
}
