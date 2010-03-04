/*
 Prescaler.cpp - Arduino Prescaler library
 Copyright (c) arms22<arms22 at gmail.com>.  All right reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 Version 001 4 Mar 2010
 */
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
