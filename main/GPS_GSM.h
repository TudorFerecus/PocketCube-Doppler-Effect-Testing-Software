#ifndef GPS_GSM_H
#define GPS_GSM_H

#include <SoftwareSerial.h>
#include <TinyGPS.h>

class GPS_GSM
{
  float gpslat, gpslon;
  TinyGPS gps;
  SoftwareSerial sgps;
  SoftwareSerial sgsm;

  public:
    GPS_GSM(int rx1, int tx1, int rx2, int tx2);
    void init_gps_gsm();

    void scan_sms();

    long get_speed();
  
};
#endif
