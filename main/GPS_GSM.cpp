#include "GPS_GSM.h"

#include <SoftwareSerial.h>
#include <TinyGPS.h>

GPS_GSM::GPS_GSM(int rx1, int tx1, int rx2, int tx2) : sgps(rx1, tx1), sgsm(rx2, tx2)
{
  
}

void GPS_GSM::init_gps_gsm()
{

  sgsm.begin(9600);
  sgps.begin(9600);
  
}


void GPS_GSM::scan_sms()
{
  GPS_GSM::sgps.listen();
    while(GPS_GSM::sgps.available())
    {
      int c = GPS_GSM::sgps.read();
      if(GPS_GSM::gps.encode(c))
      {
        float gpslat_ = GPS_GSM::gpslat;
        float gpslon_ = GPS_GSM::gpslon;
        GPS_GSM::gps.f_get_position(&gpslat_, &gpslon_);
        GPS_GSM::gpslat = gpslat_;
        GPS_GSM::gpslon = gpslon_;
      }
    }
    GPS_GSM::sgsm.listen();
      if(GPS_GSM::sgsm.available() > 0)
      {
        String c = GPS_GSM::sgsm.readString();
        c.trim();
        if(c.indexOf("GET-GPS") >= 0)
        {
          GPS_GSM::sgsm.print("\r");
          delay(1000);
          GPS_GSM::sgsm.print("AT+CMGF=1\r");
          delay(1000);

          GPS_GSM::sgsm.print("AT+CMGS=\"+40772128779\"\r");
          delay(1000);
          GPS_GSM::sgsm.print("Latitude :");
          GPS_GSM::sgsm.println(GPS_GSM::gpslat, 6);
          GPS_GSM::sgsm.print("Longitude:");
          GPS_GSM::sgsm.println(GPS_GSM::gpslon, 6);
          delay(1000);
          GPS_GSM::sgsm.write(0x1A);
          delay(1000);
        }
      }
}

long GPS_GSM::get_speed()
{
  return GPS_GSM::gps.speed();
}
