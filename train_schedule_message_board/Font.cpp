/*
  Font.cpp - Arduino library for Font
  Copyright (c) arms22. All right reserved.
  
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 */
#define __LOAD_FONTTABLE__
#include "Font.h"

bool FontClass::getBitmap(uint16_t encoding,uint8_t *w,uint8_t *h,uint8_t *bitmap)
{
	int head = 0;
	int tail = NUMBER_OF_FONTDATA - 1;
	while(head <= tail){
		int where = head + ((tail - head) / 2);
		uint16_t enc = pgm_read_word(&FontTable[where].encoding);
		if(encoding == enc){
			*w = pgm_read_byte(&FontTable[where].w);
			*h = pgm_read_byte(&FontTable[where].h);
			memcpy_P(bitmap, FontTable[where].bitmap, SIZE_OF_BITMAP);
			return true;
		}
		if(encoding < enc){
			tail = where - 1;
		}else{
			head = where + 1;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
FontClass Font = FontClass();

