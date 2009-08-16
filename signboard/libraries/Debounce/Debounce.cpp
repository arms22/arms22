
#include "WProgram.h"
#include "Debounce.h"


Debounce::Debounce(unsigned long interval_millis, uint8_t pin)
{
	interval(interval_millis);
	previous_millis = millis();
	state = digitalRead(pin);
    this->pin = pin;
}

// Contributed by Jim Schimpf
void Debounce::write(int new_state)
       {
       	state = new_state;
       	digitalWrite(pin,state);
       }

void Debounce::interval(unsigned long interval_millis)
{
  this->interval_millis = interval_millis;
}

int Debounce::update()
{
	uint8_t newState = digitalRead(pin);
	if (state != newState ) {
  		if (millis() - previous_millis >= interval_millis) {
  			previous_millis = millis();
  			state = newState;
  			return 1;
	}
  }
  
  return 0;

}

int Debounce::read()
{
	return (int)state;
}


