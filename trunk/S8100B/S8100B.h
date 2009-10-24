/*
 * CMOS TEMPERATURE COMPENSATION IC S-8100B
 */

#ifndef S8100B_H
#define S8100B_H

#include <WProgram.h>
#include <inttypes.h>

#ifndef S8100B_ADC_VREF
#define S8100B_ADC_VREF			(5000.0)
#endif

#ifndef S8100B_ZERO_CONFIG
#define S8100B_ZERO_CONFIG		(1740.0) // 3.3V 1702mV(typ), 3.0V 1695mV(typ)
#endif

#ifndef S8100B_LOW_PASS_FACTOR
#define S8100B_LOW_PASS_FACTOR	(0.9)
#endif

#define S8100B_ADC_RESOLUTION	(S8100B_ADC_VREF/1023.0)
#define S8100B_SENSITIVITY		(-8.140)

class Temperature
{
private:
	uint8_t _analogPin;
	float _lastAnalogValue;
	
public:
	Temperature(int pin)
		: _analogPin(pin)
		, _lastAnalogValue(0) {}
	
	~Temperature() {}
	
	float read() {
		if (_lastAnalogValue == 0) {
			_lastAnalogValue = analogRead(_analogPin);
		}
		
		// Low Pass Filter
		_lastAnalogValue = (_lastAnalogValue * (1.0 - S8100B_LOW_PASS_FACTOR))
			+ ((float)analogRead(_analogPin) * S8100B_LOW_PASS_FACTOR);
		
		// Calc Temp
		return ((_lastAnalogValue * S8100B_ADC_RESOLUTION) - S8100B_ZERO_CONFIG) / S8100B_SENSITIVITY;
	}
};

#endif
