#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#define	__MACRO_ONLY__
#include "fontdata.h"
#undef	__MACRO_ONLY__

class FontClass
{
public:
	bool getBitmap(uint16_t encoding,uint8_t *w,uint8_t *h,uint8_t *bitmap);
};

extern FontClass Font;

#endif
