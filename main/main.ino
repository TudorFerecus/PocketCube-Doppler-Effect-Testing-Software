//TODO: 
// EXTRACT RTC_TIMER AND TEMPERATURE INTO CLASSES IN NEW FILES
// PROGRAM THE GPS/GPRS MODULE TO SEND THE LOCATION VIA SMS
//

#include <ThreeWire.h>  
#include <Adafruit_MLX90614.h>
#include <RtcDS1302.h>
#include "SD_Card.h"
#include "Pressure.h"
#include "TempInt.h"
#define countof(a) (sizeof(a) / sizeof(a[0]))
const int IO = 0;
const int SCLK = 0;
const int CE  = 0;

ThreeWire myWire(IO,SCLK,CE); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
class RTC_Timer
{

  public:
    void init()
    {
      Serial.begin(57600);

      Serial.print("compiled: ");
      Serial.print(__DATE__);
      Serial.println(__TIME__);
  
      Rtc.Begin();
  
      RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
      printDateTime(compiled);
      Serial.println();
  
      if (!Rtc.IsDateTimeValid()) 
      {
          Serial.println("RTC lost confidence in the DateTime!");
          Rtc.SetDateTime(compiled);
      }
  
      if (Rtc.GetIsWriteProtected())
      {
          Serial.println("RTC was write protected, enabling writing now");
          Rtc.SetIsWriteProtected(false);
      }
  
      if (!Rtc.GetIsRunning())
      {
          Serial.println("RTC was not actively running, starting now");
          Rtc.SetIsRunning(true);
      }
  
      RtcDateTime now = Rtc.GetDateTime();
      if (now < compiled) 
      {
          Serial.println("RTC is older than compile time!");
          Rtc.SetDateTime(compiled);
      }
      else if (now > compiled) 
      {
          Serial.println("RTC is newer than compile time.");
      }
      else if (now == compiled) 
      {
          Serial.println("RTC is the same as compile time!");
      }
      }
      
      void get_time()
      {
        RtcDateTime now = Rtc.GetDateTime();

        printDateTime(now);
        Serial.println();
    
        if (!now.IsValid())
        {
            Serial.println("RTC lost confidence in the DateTime!");
        }
      }
      void printDateTime(const RtcDateTime& dt)
      {
          char datestring[20];
      
          snprintf_P(datestring, 
                  countof(datestring),
                  PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
                  dt.Month(),
                  dt.Day(),
                  dt.Year(),
                  dt.Hour(),
                  dt.Minute(),
                  dt.Second() );
          Serial.print(datestring);
      }
};

class Temperature
{
  Adafruit_MLX90614 mlx = Adafruit_MLX90614();
  public:
    void init()
    {
      mlx.begin();
    }

    float get_temperature()
    {
      return mlx.readObjectTempC();
    }
};
TempInt tempInt;
RTC_Timer RTC;
Temperature temperature;
Pressure pressure;
SD_Card sd_card;
void setup () 
{
    Serial.begin(9600);
    temperature.init();
    pressure.init();
    tempInt.init();
    RTC.init();
    bool sd_card_status = sd_card.init();
    if(sd_card_status)
      Serial.println("SD card init successful");
    else
      Serial.println("SD card init failed");
}

void loop () 
{
    RTC.get_time();
    float pressure_now = pressure.get_pressure();
    float temperature_now = temperature.get_temperature();
    float uv_now = analogRead(A0) * (5.0 / 1023.0) * 0.1;
    float temp_int_now = tempInt.get_temp();

    sd_card.write_to_file(String("Pressure(Pa):") + String(pressure_now) + String(" Temperature: ") + String(temperature_now))
    + String(" UV: ") + String(uv_now) + String(" Temp Int: ") + String(temp_int_now);
    
    Serial.print("Pressure(Pa):");
    Serial.println(pressure_now);
    Serial.print("Temperature: ");
    Serial.println(temperature_now);
    delay(5000);
}
