#ifndef ColorSensor_h
#define ColorSensor_h

#include <inttypes.h>

class ColorSensor
{
	uint8_t _pinOUT;
	uint8_t _pinS2;
	uint8_t _pinS3;
	
	uint8_t _colorReference;
	
	uint32_t _rFreq;
	uint32_t _gFreq;
	uint32_t _bFreq;
	
	struct _freq_range {
		uint32_t min;
		uint32_t max;
	};
	
	struct _freq_range _rfreqRange;
	struct _freq_range _gfreqRange;
	struct _freq_range _bfreqRange;
	
	static uint32_t measureSquareWaveWidth(uint8_t pin);
	
public:
	ColorSensor(uint8_t pinOUT,uint8_t pinS2,uint8_t pinS3);
	~ColorSensor();
	
	enum {
		COLOR_REF_RAW,			// 100Hz - 600,000Hz
		COLOR_REF_MANUAL,		// 0 - 1023
		COLOR_REF_WEAK_LIGHT,	// 0 - 1023
		COLOR_REF_STRONG_LIGHT,	// 0 - 1023
		COLOR_REF_REFLECTED_LIGHT_NEAR,		// 0 - 1023
		COLOR_REF_REFLECTED_LIGHT_FAR,		// 0 - 1023
	};
	
	void colorReference(uint8_t cref);
	void adjustWhite(void);
	void adjustBlack(void);
	void measureColor(void);
	
	uint32_t redValue(void);
	uint32_t greenValue(void);
	uint32_t blueValue(void);
};

#endif
