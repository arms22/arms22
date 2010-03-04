#ifndef Prescaler_h
#define Prescaler_h

#include <inttypes.h>
#include <avr/power.h>

unsigned long scaledMillis(void);
void scaledDelay(unsigned long ms);

class PrescalerClass
{
  static clock_div_t _division;
  friend unsigned long scaledMillis(void);
  friend void scaledDelay(unsigned long ms);
public:
  //  typedef enum {
  //    clock_div_1 = 0,
  //    clock_div_2 = 1,
  //    clock_div_4 = 2,
  //    clock_div_8 = 3,
  //    clock_div_16 = 4,
  //    clock_div_32 = 5,
  //    clock_div_64 = 6,
  //    clock_div_128 = 7,
  //    clock_div_256 = 8
  //  } clock_div_t;
  static void set(clock_div_t div);
  static clock_div_t division(void);
};

extern PrescalerClass Prescaler;

#endif
