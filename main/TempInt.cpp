#include "TempInt.h"

#define ALTITUDE 66.0

bool TempInt::init()
{
  if(TempInt::pressure.begin())
    return true;
   else return false;
}

float TempInt::get_temp()
{
  char status;
  double T,P,p0,a;
  status = pressure.startTemperature();
  if(status != 0)
  {
    status = pressure.getTemperature(T);
    if(status != 0)
      return (9.0/5.0)*T+32.0,2;
  }
  return 0;
}
