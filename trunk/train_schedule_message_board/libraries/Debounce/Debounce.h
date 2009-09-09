

#ifndef Debounce_h
#define Debounce_h

#include <inttypes.h>

class Debounce
{

public:
  Debounce(unsigned long interval_millis, uint8_t pin );
  void interval(unsigned long interval_millis);
  int update();
  int read();
  // Contributed by Jim Schimpf
  void write(int new_state);
  
private:
  unsigned long  previous_millis, interval_millis;
  uint8_t state;
  uint8_t pin;
};

#endif


