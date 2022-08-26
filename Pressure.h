#ifndef PRESSURE_H
#define PRESSURE_H

#include <Wire.h>
#include "SparkFunMPL3115A2.h"

class Pressure
{
  MPL3115A2 myPressure;
  public:
    void init();
    float get_pressure();
};

#endif
