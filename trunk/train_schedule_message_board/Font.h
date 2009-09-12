#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include "fontdata.h"

class FontClass
{
public:
	bool getBitmap(uint16_t encoding,uint8_t *w,uint8_t *h,uint8_t *bitmap);
};

extern FontClass Font;

#endif
