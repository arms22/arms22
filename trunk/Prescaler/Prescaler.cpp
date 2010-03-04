#include <WProgram.h>
#include "Prescaler.h"

clock_div_t PrescalerClass::_division = clock_div_1;

void PrescalerClass::set(clock_div_t div)
{
  _division = div;
  clock_prescale_set(div);
}

clock_div_t PrescalerClass::division()
{
  return _division;
}

PrescalerClass Prescaler;

unsigned long scaledMillis(void)
{
  unsigned long m = millis();
  return m << PrescalerClass::_division;
}

void scaledDelay(unsigned long ms)
{
  unsigned long end = scaledMillis() + ms;
  while (scaledMillis() < end)
    ;
}
