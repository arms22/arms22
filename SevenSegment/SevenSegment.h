#ifndef SevenSegment_h
#define SevenSegment_h

#include <inttypes.h>

#define NUMBER_OF_MAXDIGITS    14	/* YYYY.MM.DD hh:mm:ss */
#define NUMBER_OF_SEGMENTS     8

#define SEG_A_BIT  0x01
#define SEG_B_BIT  0x02
#define SEG_C_BIT  0x04
#define SEG_D_BIT  0x08
#define SEG_E_BIT  0x10
#define SEG_F_BIT  0x20
#define SEG_G_BIT  0x40
#define SEG_DP_BIT 0x80

class SevenSegment
{
private:
	unsigned long _lastUpdateTime;
	uint16_t _updateInterval;
	uint8_t _digitPins[NUMBER_OF_MAXDIGITS];
	uint8_t _segmentPins[NUMBER_OF_SEGMENTS];
	uint8_t _numericData[NUMBER_OF_MAXDIGITS];
	uint8_t _numberOfDigits;
	uint8_t _numberOfSegments;
	uint8_t _currentDigit;
	//uint8_t _anodeCommon;
	void init(uint8_t digits[],uint8_t segments[]);
public:
	SevenSegment(uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4,
				 uint8_t segA, uint8_t segB, uint8_t segC, uint8_t segD,
				 uint8_t segE, uint8_t segF, uint8_t segG, uint8_t segDP);
	void begin(void);
	void write(uint8_t digit,uint8_t value);
	void digitOn(void);
	void digitOff(void);
	void updateDigit(void);
	bool update(void);
	void print(long value);
	static const uint8_t NUMERICAL_NUMBER[10];
};

#endif
