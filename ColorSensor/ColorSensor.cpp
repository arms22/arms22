#include "WConstants.h"
#include "ColorSensor.h"
#include "pins_arduino.h"

#define COLOR_SENSOR_MAX_FREQ			(600000)
#define COLOR_SENSOR_MIN_FREQ			(5)
#define COLOR_SENSOR_MEASURE_TIMEOUT	(100000)
#define COLOR_SENSOR_ADJUST_SAMPLES		(8)

ColorSensor::ColorSensor(uint8_t pinOUT,uint8_t pinS2,uint8_t pinS3)
{
	_pinOUT = pinOUT;
	_pinS2	= pinS2;
	_pinS3	= pinS3;
	
	_colorReference = ColorSensor::COLOR_REF_RAW;
	
	_rFreq  = 0;
	_gFreq  = 0;
	_bFreq  = 0;
	
	_rfreqRange.max = COLOR_SENSOR_MAX_FREQ;
	_rfreqRange.min = COLOR_SENSOR_MIN_FREQ;
	
	_gfreqRange.max = COLOR_SENSOR_MAX_FREQ;
	_gfreqRange.min = COLOR_SENSOR_MIN_FREQ;
	
	_bfreqRange.max = COLOR_SENSOR_MAX_FREQ;
	_bfreqRange.min = COLOR_SENSOR_MIN_FREQ;
	
	pinMode(_pinOUT,INPUT);
	pinMode(_pinS2, OUTPUT);
	pinMode(_pinS3, OUTPUT);
	
	digitalWrite(_pinS2, LOW);
	digitalWrite(_pinS3, LOW);
}

ColorSensor::~ColorSensor()
{
}

#define median(a,b,c)	((a > b) ? (a <= c ? a : b > c ? b : c) : (b <= c ? b : a > c ? a : c))

uint32_t ColorSensor::measureSquareWaveWidth(uint8_t pin)
{
// 	uint32_t samples[3];
	
// 	samples[0] = 0;
// 	samples[1] = 0;
// 	samples[2] = 0;
	
// 	{
// 		uint8_t bit = digitalPinToBitMask(pin);
// 		volatile uint8_t *reg = portInputRegister(digitalPinToPort(pin));
// 		uint8_t stateMask;
// 		uint32_t numloops = 0;
// 		uint32_t maxloops = microsecondsToClockCycles(COLOR_SENSOR_MEASURE_TIMEOUT) / 16;
		
// 		stateMask = (*reg & bit);
// 		while ((*reg & bit) == stateMask)
// 			if (numloops++ == maxloops)
// 				return COLOR_SENSOR_MEASURE_TIMEOUT;
		
// 		while ((*reg & bit) != stateMask)
// 			samples[0]++;
		
// 		while ((*reg & bit) == stateMask)
// 			samples[1]++;
		
// 		while ((*reg & bit) != stateMask)
// 			samples[2]++;
// 	}
	
// 	return clockCyclesToMicroseconds(median(samples[0],samples[1],samples[2]) * 10 + 16);
	
	uint32_t duration = pulseIn(pin, !digitalRead(pin), COLOR_SENSOR_MEASURE_TIMEOUT);
	return (duration ? duration : COLOR_SENSOR_MEASURE_TIMEOUT);
}

void ColorSensor::colorReference(uint8_t cref)
{
	_colorReference = cref;
}

void ColorSensor::adjustWhite(void)
{
	int i;
	uint32_t r_average = 0;
	uint32_t g_average = 0;
	uint32_t b_average = 0;
	
	for(i=0;i<COLOR_SENSOR_ADJUST_SAMPLES;i++){
		measureColor();
		r_average += _rFreq;
		g_average += _gFreq;
		b_average += _bFreq;
	}
	
	r_average /= COLOR_SENSOR_ADJUST_SAMPLES;
	_rfreqRange.max = (r_average > _rfreqRange.min) ? r_average : _rfreqRange.max;
	
	g_average /= COLOR_SENSOR_ADJUST_SAMPLES;
	_gfreqRange.max = (g_average > _gfreqRange.min) ? g_average : _gfreqRange.max;
	
	b_average /= COLOR_SENSOR_ADJUST_SAMPLES;
	_bfreqRange.max = (b_average > _bfreqRange.min) ? b_average : _bfreqRange.max;
}

void ColorSensor::adjustBlack(void)
{
	int i;
	uint32_t r_average = 0;
	uint32_t g_average = 0;
	uint32_t b_average = 0;
	
	for(i=0;i<COLOR_SENSOR_ADJUST_SAMPLES;i++){
		measureColor();
		r_average += _rFreq;
		g_average += _gFreq;
		b_average += _bFreq;
	}
	
	r_average /= COLOR_SENSOR_ADJUST_SAMPLES;
	_rfreqRange.min = (r_average < _rfreqRange.max) ? r_average : _rfreqRange.min;
	
	g_average /= COLOR_SENSOR_ADJUST_SAMPLES;
	_gfreqRange.min = (g_average < _gfreqRange.max) ? g_average : _gfreqRange.min;
	
	b_average /= COLOR_SENSOR_ADJUST_SAMPLES;
	_bfreqRange.min = (b_average < _bfreqRange.max) ? b_average : _bfreqRange.min;
}

void ColorSensor::measureColor(void)
{
	// measure red freq
	digitalWrite(_pinS2, LOW);
	digitalWrite(_pinS3, LOW);
	_rFreq = 1000000UL / 2UL / ColorSensor::measureSquareWaveWidth(_pinOUT);
	
	// measure blue freq
	digitalWrite(_pinS3, HIGH);
	_bFreq = 1000000UL / 2UL / ColorSensor::measureSquareWaveWidth(_pinOUT);
	
	// measure green freq
	digitalWrite(_pinS2, HIGH);
	_gFreq = 1000000UL / 2UL / ColorSensor::measureSquareWaveWidth(_pinOUT);
}

uint32_t ColorSensor::redValue(void)
{
	if(_colorReference != ColorSensor::COLOR_REF_RAW){
		return ((constrain(_rFreq,_rfreqRange.min,_rfreqRange.max) - _rfreqRange.min) << 10) / (_rfreqRange.max - _rfreqRange.min);
	}
	return _rFreq;
}

uint32_t ColorSensor::greenValue(void)
{
	if(_colorReference != ColorSensor::COLOR_REF_RAW){
		return ((constrain(_gFreq,_gfreqRange.min,_gfreqRange.max) - _gfreqRange.min) << 10) / (_gfreqRange.max - _gfreqRange.min);
	}
	return _gFreq;
}

uint32_t ColorSensor::blueValue(void)
{
	if(_colorReference != ColorSensor::COLOR_REF_RAW){
		return ((constrain(_bFreq,_bfreqRange.min,_bfreqRange.max) - _bfreqRange.min) << 10) / (_bfreqRange.max - _bfreqRange.min);
	}
	return _bFreq;
}
