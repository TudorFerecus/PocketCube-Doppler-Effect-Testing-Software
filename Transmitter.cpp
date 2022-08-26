#include "Transmitter.h"

#include <SPI.h>      
#include <Mirf.h>    
#include <nRF24L01.h> 
#include <MirfHardwareSpiDriver.h> 

Transmitter::Transmitter(int cePin_, int csnPin_)
{
  cePin = cePin_;
  csnPin = csnPin_;
}

void Transmitter::init_transmitter()
{
  Mirf.cePin = cePin; 
  Mirf.csnPin = csnPin; 
  Mirf.spi = &MirfHardwareSpi; 
  Mirf.init();  
  Mirf.channel = 1; 
  Mirf.payload = sizeof(float); 
  Mirf.config(); 
  Mirf.setTADDR((byte *) "radio"); 
  Mirf.setRADDR((byte *) "radi1");
}

void Transmitter::send_wave()
{
  //it changes based off which pin it is on in the satellite
  float value = analogRead(A0); 
  Serial.println(value);
  
  Mirf.send((byte *) &value); 
  while(Mirf.isSending());
}
