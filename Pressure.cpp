#include "Pressure.h"

#include <Wire.h>
#include "SparkFunMPL3115A2.h"

void Pressure::init()
{
  Wire.begin();
  Pressure::myPressure.begin();
  Pressure::myPressure.setModeBarometer(); 
  Pressure::myPressure.setOversampleRate(7);
  Pressure::myPressure.enableEventFlags();
  
}

float Pressure::get_pressure()
{
  float prs = Pressure::myPressure.readPressure();
  return prs;
}
