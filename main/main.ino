#define countof(a) (sizeof(a) / sizeof(a[0]))
#include "SD_Card.h"
#include "Pressure.h"
#include "Transmitter.h"
#include "GPS_GSM.h"
#include <Wire.h> 
#include <RtcDS3231.h>
#include <Adafruit_MLX90614.h>
RtcDS3231<TwoWire> Rtc(Wire);
#include <SPI.h>       

class RTC_Timer
{
  public:
    void init_clock () 
    {
        Serial.print("compiled: ");
        Serial.print(__DATE__);
        Serial.println(__TIME__);
    
        Rtc.Begin();
    
        RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
        printDateTime(compiled);
        Serial.println();
    
        if (!Rtc.IsDateTimeValid()) 
        {
            if (Rtc.LastError() != 0)
            {
                Serial.print("RTC communications error = ");
                Serial.println(Rtc.LastError());
            }
            else
            {
                Serial.println("RTC lost confidence in the DateTime!");
                Rtc.SetDateTime(compiled);
            }
        }
    
        if (!Rtc.GetIsRunning())
        {
            Serial.println("RTC was not actively running, starting now");
            Rtc.SetIsRunning(true);
        }
    
        RtcDateTime now = Rtc.GetDateTime();
    
        Rtc.Enable32kHzPin(false);
        Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone); 
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

    String get_date_time()
    {
      RtcDateTime dt = Rtc.GetDateTime();
    
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
        return datestring;
    }

    int get_seconds()
    {
      RtcDateTime dt = Rtc.GetDateTime();
      return dt.Second() + dt.Minute() * 60 + dt.Hour() * 3600;
    }
};

class Temperature
{
  Adafruit_MLX90614 mlx = Adafruit_MLX90614();
  public:
    void init_temp()
    {
      mlx.begin();
    }

    float get_temperature()
    {
      return mlx.readObjectTempC();
    }
};


Temperature temperature;
RTC_Timer RTC;
SD_Card sd_card;
GPS_GSM gps_gsm(2,4,3,6);

int time_last_write;
Pressure pressure;
//it changes based off the pins it is on in the satellite
Transmitter transmitter(9, 10);

//moment of maximum velocity in seconds (it will change after further analysing rocket's path)
int max_velocity_moment = 40000;
int time_start;
bool send_wave = false;

void setup()
{
  Serial.begin(9600);
  RTC.init_clock();
  temperature.init_temp();
  transmitter.init_transmitter();
  gps_gsm.init_gps_gsm();
  bool sd_card_status = sd_card.init_sd();
    if(sd_card_status)
      Serial.println("SD card init successful");
    else
      Serial.println("SD card init failed");
  time_start = RTC.get_seconds();
  
}

void loop()
{
  int time_now = millis();
  if(time_now - time_last_write > 1000)
  {
    float temperature_now = temperature.get_temperature();
    float pressure_now = pressure.get_pressure();
    sd_card.write_to_file(RTC.get_date_time() + String(" Pressure(Pa): ") + String(pressure_now) + String(" Temperature: ") + String(temperature_now));
    time_last_write = time_now;
  
    if(gps_gsm.get_speed() == 0)
    {
      gps_gsm.scan_sms()
    }
      
  }

  if(time_now - time_start > max_velocity_moment && !send_wave)
  {
    float temperature_now = temperature.get_temperature();
    float pressure_now = pressure.get_pressure();
    sd_card.write_to_file(RTC.get_date_time() + String(" Pressure(Pa): ") + String(pressure_now) + String(" Temperature: ") + String(temperature_now) +
        String(" WAVE SENT!!") + String(" Speed: ") + String(gps_gsm.get_speed()));
    send_wave = true;
    transmitter.send_wave();
  }
  
  
}
