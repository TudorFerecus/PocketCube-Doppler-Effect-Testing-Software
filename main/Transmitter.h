#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include <SPI.h>      
#include <Mirf.h>    
#include <nRF24L01.h> 
#include <MirfHardwareSpiDriver.h> 

class Transmitter
{
  int cePin;
  int csnPin;
  public:
    Transmitter(int cePin_, int csnPin_);
    void init_transmitter();

    void send_wave();
};
#endif
