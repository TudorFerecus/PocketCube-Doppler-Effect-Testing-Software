#ifndef SD_CARD_H
#define SD_CARD_H
#include <SD.h>
#include <SPI.h>
class SD_Card
{
  File file;
  public:
    bool init_sd();

    bool write_to_file(String str);
};
#endif
