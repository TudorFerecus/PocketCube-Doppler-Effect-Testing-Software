#ifndef TEMPINT_H
#define TEMPINT_H
#include <SFE_BMP180.h>
#include <Wire.h>
class TempInt
{
  SFE_BMP180 pressure;
  public:
    bool init();

    float get_temp();
};
#endif
